/*
 * Copyright (C) 2006-2008	Justin Karneges
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
 * CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
 * TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
 * SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

// Note: QJDnsShared supports multiple interfaces for multicast, but only one
//   for IPv4 and one for IPv6.  Sharing multiple interfaces of the same IP
//   version for multicast is unfortunately not possible without reworking
//   the jdns subsystem.
//
//   The reason for this limitation is that in order to do multi-interface
//   multicast, you have to do a single bind to Any, and then use special
//   functions to determine which interface a packet came from and to
//   specify which interface a packet should go out on.  Again this is just
//   not possible with the current system and the assumptions made by jdns.

// Note: When quering against multiple interfaces with multicast, it is
//   possible that different answers for a unique record may be reported
//   on each interface.  We don't do anything about this.

#include "qjdnsshared_p.h"

// for caching system info

class SystemInfoCache
{
public:
	QJDns::SystemInfo info;
	QTime time;
};

Q_GLOBAL_STATIC(QMutex, jdnsshared_mutex)
Q_GLOBAL_STATIC(SystemInfoCache, jdnsshared_infocache)

static QJDns::SystemInfo get_sys_info()
{
	QMutexLocker locker(jdnsshared_mutex());
	SystemInfoCache *c = jdnsshared_infocache();

	// cache info for 1/2 second, enough to prevent re-reading of sys
	//   info 20 times because of all the different resolves
	if(c->time.isNull() || c->time.elapsed() >= 500)
	{
		c->info = QJDns::systemInfo();
		c->time.start();
	}

	return c->info;
}

static bool domainCompare(const QByteArray &a, const QByteArray &b)
{
	return (qstricmp(a.data(), b.data()) == 0) ? true: false;
}

// adapted from jdns_mdnsd.c, _a_match()
static bool matchRecordExceptTtl(const QJDns::Record &a, const QJDns::Record &b)
{
	if(a.type != b.type || !domainCompare(a.owner, b.owner))
		return false;

	if(a.type == QJDns::Srv)
	{
		if(domainCompare(a.name, b.name)
			&& a.port == b.port
			&& a.priority == b.priority
			&& a.weight == b.weight)
		{
			return true;
		}
	}
	else if(a.type == QJDns::Ptr || a.type == QJDns::Ns || a.type == QJDns::Cname)
	{
		if(domainCompare(a.name, b.name))
			return true;
	}
	else if(a.rdata == b.rdata)
		return true;

	return false;
}

static void getHex(unsigned char in, char *hi, char *lo)
{
	QString str;
	str.sprintf("%02x", in);
	*hi = str[0].toLatin1();
	*lo = str[1].toLatin1();
}

static QByteArray getDec(int in)
{
	return QString::number(in).toLatin1();
}

static QByteArray makeReverseName(const QHostAddress &addr)
{
	QByteArray out;

	if(addr.protocol() == QAbstractSocket::IPv6Protocol)
	{
		Q_IPV6ADDR raw = addr.toIPv6Address();
		for(int n = 0; n < 32; ++n)
		{
			char hi, lo;
			getHex(raw.c[31 - n], &hi, &lo);
			out += lo;
			out += '.';
			out += hi;
			out += '.';
		}
		out += "ip6.arpa.";
	}
	else
	{
		quint32 rawi = addr.toIPv4Address();
		int raw[4];
		raw[0] = (rawi >> 24) & 0xff;
		raw[1] = (rawi >> 16) & 0xff;
		raw[2] = (rawi >>  8) & 0xff;
		raw[3] = rawi & 0xff;
		for(int n = 0; n < 4; ++n)
		{
			out += getDec(raw[3 - n]);
			out += '.';
		}
		out += "in-addr.arpa.";
	}

	return out;
}

// adapted from qHash<QPair>
static inline uint qHash(const Handle &key)
{
	uint h1 = ::qHash(key.jdns);
	uint h2 = ::qHash(key.id);
	return ((h1 << 16) | (h1 >> 16)) ^ h2;
}


//----------------------------------------------------------------------------
// JDnsShutdown
//----------------------------------------------------------------------------
void JDnsShutdownAgent::start()
{
	QMetaObject::invokeMethod(this, "started", Qt::QueuedConnection);
}

JDnsShutdownWorker::JDnsShutdownWorker(const QList<QJDnsShared*> &_list) : QObject(0), list(_list)
{
	foreach(QJDnsShared *i, list)
	{
		connect(i, SIGNAL(shutdownFinished()), SLOT(jdns_shutdownFinished()));
		i->shutdown(); // MUST support DOR-DS, and it does
	}
}

void JDnsShutdownWorker::jdns_shutdownFinished()
{
	QJDnsShared *i = (QJDnsShared *)sender();
	list.removeAll(i);
	delete i;
	if(list.isEmpty())
		emit finished();
}

void JDnsShutdown::waitForShutdown(const QList<QJDnsShared*> &_list)
{
	list = _list;
	phase = 0;

	m.lock();
	start();
	w.wait(&m);

	foreach(QJDnsShared *i, list)
	{
		i->setParent(0);
		i->moveToThread(this);
	}

	phase = 1;
	agent->start();
	wait();
}

void JDnsShutdown::run()
{
	m.lock();
	agent = new JDnsShutdownAgent;
	connect(agent, SIGNAL(started()), SLOT(agent_started()), Qt::DirectConnection);
	agent->start();
	exec();
	delete agent;
}

void JDnsShutdown::agent_started()
{
	if(phase == 0)
	{
		w.wakeOne();
		m.unlock();
	}
	else
	{
		worker = new JDnsShutdownWorker(list);
		connect(worker, SIGNAL(finished()), SLOT(worker_finished()), Qt::DirectConnection);
	}
}

void JDnsShutdown::worker_finished()
{
	delete worker;
	worker = 0;

	quit();
}

//----------------------------------------------------------------------------
// QJDnsSharedDebug
//----------------------------------------------------------------------------

QJDnsSharedDebugPrivate::QJDnsSharedDebugPrivate(QJDnsSharedDebug *_q)
	: QObject(_q)
	, q(_q)
{
	dirty = false;
}

void QJDnsSharedDebugPrivate::addDebug(const QString &name, const QStringList &_lines)
{
	if(!_lines.isEmpty())
	{
		QMutexLocker locker(&m);
		for(int n = 0; n < _lines.count(); ++n)
			lines += name + ": " + _lines[n];
		if(!dirty)
		{
			dirty = true;
			QMetaObject::invokeMethod(this, "doUpdate", Qt::QueuedConnection);
		}
	}
}

void QJDnsSharedDebugPrivate::doUpdate()
{
	{
		QMutexLocker locker(&m);
		if(!dirty)
			return;
	}
	emit q->readyRead();
}

QJDnsSharedDebug::QJDnsSharedDebug(QObject *parent)
:QObject(parent)
{
	d = new QJDnsSharedDebugPrivate(this);
}

QJDnsSharedDebug::~QJDnsSharedDebug()
{
	delete d;
}

QStringList QJDnsSharedDebug::readDebugLines()
{
	QMutexLocker locker(&d->m);
	QStringList tmplines = d->lines;
	d->lines.clear();
	d->dirty = false;
	return tmplines;
}

//----------------------------------------------------------------------------
// QJDnsSharedRequest
//----------------------------------------------------------------------------
QJDnsSharedPrivate::QJDnsSharedPrivate(QJDnsShared *_q)
	: QObject(_q)
	, q(_q)
{
}

QJDnsSharedRequest *QJDnsSharedPrivate::findRequest(QJDns *jdns, int id) const
{
	Handle h(jdns, id);
	return requestForHandle.value(h);
}

void QJDnsSharedPrivate::jdns_link(QJDns *jdns)
{
	connect(jdns, SIGNAL(resultsReady(int,QJDns::Response)), SLOT(jdns_resultsReady(int,QJDns::Response)));
	connect(jdns, SIGNAL(published(int)), SLOT(jdns_published(int)));
	connect(jdns, SIGNAL(error(int,QJDns::Error)), SLOT(jdns_error(int,QJDns::Error)));
	connect(jdns, SIGNAL(shutdownFinished()), SLOT(jdns_shutdownFinished()));
	connect(jdns, SIGNAL(debugLinesReady()), SLOT(jdns_debugLinesReady()));
}

int QJDnsSharedPrivate::getNewIndex() const
{
	// find lowest unused value
	for(int n = 0;; ++n)
	{
		bool found = false;
		foreach(Instance *i, instances)
		{
			if(i->index == n)
			{
				found = true;
				break;
			}
		}
		if(!found)
			return n;
	}
}

void QJDnsSharedPrivate::addDebug(int index, const QString &line)
{
	if(db)
		db->d->addDebug(dbname + QString::number(index), QStringList() << line);
}

void QJDnsSharedPrivate::doDebug(QJDns *jdns, int index)
{
	QStringList lines = jdns->debugLines();
	if(db)
		db->d->addDebug(dbname + QString::number(index), lines);
}

QJDnsSharedPrivate::PreprocessMode QJDnsSharedPrivate::determinePpMode(const QJDns::Record &in)
{
	// Note: since our implementation only allows 1 ipv4 and 1 ipv6
	//   interface to exist, it is safe to publish both kinds of
	//   records on both interfaces, with the same values.	For
	//   example, an A record can be published on both interfaces,
	//   with the value set to the ipv4 interface.	If we supported
	//   multiple ipv4 interfaces, then this wouldn't work, because
	//   we wouldn't know which value to use for the A record when
	//   publishing on the ipv6 interface.

	// publishing our own IP address?  null address means the user
	//   wants us to fill in the blank with our address.
	if((in.type == QJDns::Aaaa || in.type == QJDns::A) && in.address.isNull())
	{
		return FillInAddress;
	}
	// publishing our own reverse lookup?  partial owner means
	//   user wants us to fill in the rest.
	else if(in.type == QJDns::Ptr && in.owner == ".ip6.arpa.")
	{
		return FillInPtrOwner6;
	}
	else if(in.type == QJDns::Ptr && in.owner == ".in-addr.arpa.")
	{
		return FillInPtrOwner4;
	}

	return None;
}

QJDns::Record QJDnsSharedPrivate::manipulateRecord(const QJDns::Record &in, PreprocessMode ppmode, bool *modified)
{
	if(ppmode == FillInAddress)
	{
		QJDns::Record out = in;

		if(in.type == QJDns::Aaaa)
		{
			// are we operating on ipv6?
			foreach(Instance *i, instances)
			{
				if(i->addr.protocol() == QAbstractSocket::IPv6Protocol)
				{
					if(modified && !(out.address == i->addr))
						*modified = true;
					out.address = i->addr;
					break;
				}
			}
		}
		else // A
		{
			// are we operating on ipv4?
			foreach(Instance *i, instances)
			{
				if(i->addr.protocol() == QAbstractSocket::IPv4Protocol)
				{
					if(modified && !(out.address == i->addr))
						*modified = true;
					out.address = i->addr;
					break;
				}
			}
		}

		return out;
	}
	else if(ppmode == FillInPtrOwner6)
	{
		QJDns::Record out = in;

		// are we operating on ipv6?
		foreach(Instance *i, instances)
		{
			if(i->addr.protocol() == QAbstractSocket::IPv6Protocol)
			{
				QByteArray newOwner = makeReverseName(i->addr);
				if(modified && !(out.owner == newOwner))
					*modified = true;
				out.owner = newOwner;
				break;
			}
		}

		return out;
	}
	else if(ppmode == FillInPtrOwner4)
	{
		QJDns::Record out = in;

		// are we operating on ipv4?
		foreach(Instance *i, instances)
		{
			if(i->addr.protocol() == QAbstractSocket::IPv4Protocol)
			{
				QByteArray newOwner = makeReverseName(i->addr);
				if(modified && !(out.owner == newOwner))
					*modified = true;
				out.owner = newOwner;
				break;
			}
		}

		return out;
	}

	if(modified)
		*modified = false;
	return in;
}

void QJDnsSharedPrivate::late_shutdown()
{
	shutting_down = false;
	emit q->shutdownFinished();
}

QJDnsSharedRequestPrivate::QJDnsSharedRequestPrivate(QJDnsSharedRequest *_q) : QObject(_q), q(_q), lateTimer(this)
{
	connect(&lateTimer, SIGNAL(timeout()), SLOT(lateTimer_timeout()));
}

void QJDnsSharedRequestPrivate::resetSession()
{
	name = QByteArray();
	pubrecord = QJDns::Record();
	handles.clear();
	published.clear();
	queryCache.clear();
}

void QJDnsSharedRequestPrivate::lateTimer_timeout()
{
	emit q->resultsReady();
}

QJDnsSharedRequest::QJDnsSharedRequest(QJDnsShared *jdnsShared, QObject *parent)
:QObject(parent)
{
	d = new QJDnsSharedRequestPrivate(this);
	d->jsp = jdnsShared->d;
}

QJDnsSharedRequest::~QJDnsSharedRequest()
{
	cancel();
	delete d;
}

QJDnsSharedRequest::Type QJDnsSharedRequest::type()
{
	return d->type;
}

void QJDnsSharedRequest::query(const QByteArray &name, int type)
{
	cancel();
	d->jsp->queryStart(this, name, type);
}

void QJDnsSharedRequest::publish(QJDns::PublishMode m, const QJDns::Record &record)
{
	cancel();
	d->jsp->publishStart(this, m, record);
}

void QJDnsSharedRequest::publishUpdate(const QJDns::Record &record)
{
	// only allowed to update if we have an active publish
	if(!d->handles.isEmpty() && d->type == Publish)
		d->jsp->publishUpdate(this, record);
}

void QJDnsSharedRequest::cancel()
{
	d->lateTimer.stop();
	if(!d->handles.isEmpty())
	{
		if(d->type == Query)
			d->jsp->queryCancel(this);
		else
			d->jsp->publishCancel(this);
	}
	d->resetSession();
}

bool QJDnsSharedRequest::success() const
{
	return d->success;
}

QJDnsSharedRequest::Error QJDnsSharedRequest::error() const
{
	return d->error;
}

QList<QJDns::Record> QJDnsSharedRequest::results() const
{
	return d->results;
}

//----------------------------------------------------------------------------
// QJDnsShared
//----------------------------------------------------------------------------
QJDnsShared::QJDnsShared(Mode mode, QObject *parent)
:QObject(parent)
{
	d = new QJDnsSharedPrivate(this);
	d->mode = mode;
	d->shutting_down = false;
	d->db = 0;
}

QJDnsShared::~QJDnsShared()
{
	foreach(QJDnsSharedPrivate::Instance *i, d->instances)
	{
		delete i->jdns;
		delete i;
	}
	delete d;
}

void QJDnsShared::setDebug(QJDnsSharedDebug *db, const QString &name)
{
	d->db = db;
	d->dbname = name;
}

bool QJDnsShared::addInterface(const QHostAddress &addr)
{
	return d->addInterface(addr);
}

void QJDnsShared::removeInterface(const QHostAddress &addr)
{
	d->removeInterface(addr);
}

void QJDnsShared::shutdown()
{
	d->shutting_down = true;
	if(!d->instances.isEmpty())
	{
		foreach(QJDnsSharedPrivate::Instance *i, d->instances)
			i->jdns->shutdown();
	}
	else
		QMetaObject::invokeMethod(d, "late_shutdown", Qt::QueuedConnection);
}

QList<QByteArray> QJDnsShared::domains()
{
	return get_sys_info().domains;
}

void QJDnsShared::waitForShutdown(const QList<QJDnsShared*> &instances)
{
	JDnsShutdown s;
	s.waitForShutdown(instances);
}

bool QJDnsSharedPrivate::addInterface(const QHostAddress &addr)
{
	if(shutting_down)
		return false;

	// make sure we don't have this one already
	foreach(Instance *i, instances)
	{
		if(i->addr == addr)
			return false;
	}

	int index = getNewIndex();
	addDebug(index, QString("attempting to use interface %1").arg(addr.toString()));

	QJDns *jdns;

	if(mode == QJDnsShared::UnicastInternet || mode == QJDnsShared::UnicastLocal)
	{
		jdns = new QJDns(this);
		jdns_link(jdns);
		if(!jdns->init(QJDns::Unicast, addr))
		{
			doDebug(jdns, index);
			delete jdns;
			return false;
		}

		if(mode == QJDnsShared::UnicastLocal)
		{
			QJDns::NameServer host;
			if(addr.protocol() == QAbstractSocket::IPv6Protocol)
				host.address = QHostAddress("FF02::FB");
			else
				host.address = QHostAddress("224.0.0.251");
			host.port = 5353;
			jdns->setNameServers(QList<QJDns::NameServer>() << host);
		}
	}
	else // Multicast
	{
		// only one multicast interface allowed per IP protocol version.
		// this is because we bind to INADDR_ANY.

		bool have_v6 = false;
		bool have_v4 = false;
		foreach(Instance *i, instances)
		{
			if(i->addr.protocol() == QAbstractSocket::IPv6Protocol)
				have_v6 = true;
			else
				have_v4 = true;
		}

		bool is_v6 = (addr.protocol() == QAbstractSocket::IPv6Protocol) ? true : false;

		if(is_v6 && have_v6)
		{
			addDebug(index, "already have an ipv6 interface");
			return false;
		}

		if(!is_v6 && have_v4)
		{
			addDebug(index, "already have an ipv4 interface");
			return false;
		}

		QHostAddress actualBindAddress;
		if(is_v6)
			actualBindAddress = QHostAddress::AnyIPv6;
		else
			actualBindAddress = QHostAddress::Any;

		jdns = new QJDns(this);
		jdns_link(jdns);
		if(!jdns->init(QJDns::Multicast, actualBindAddress))
		{
			doDebug(jdns, index);
			delete jdns;
			return false;
		}
	}

	Instance *i = new Instance;
	i->jdns = jdns;
	i->addr = addr;
	i->index = index;
	instances += i;
	instanceForQJDns.insert(i->jdns, i);

	addDebug(index, "interface ready");

	if(mode == QJDnsShared::Multicast)
	{
		// extend active requests to this interface
		foreach(QJDnsSharedRequest *obj, requests)
		{
			if(obj->d->type == QJDnsSharedRequest::Query)
			{
				Handle h(i->jdns, i->jdns->queryStart(obj->d->name, obj->d->qType));
				obj->d->handles += h;
				requestForHandle.insert(h, obj);
			}
			else // Publish
			{
				bool modified;
				obj->d->pubrecord = manipulateRecord(obj->d->pubrecord, obj->d->ppmode, &modified);
				// if the record changed, update on the other (existing) interfaces
				if(modified)
				{
					foreach(Handle h, obj->d->handles)
						h.jdns->publishUpdate(h.id, obj->d->pubrecord);
				}

				// publish the record on the new interface
				Handle h(i->jdns, i->jdns->publishStart(obj->d->pubmode, obj->d->pubrecord));
				obj->d->handles += h;
				requestForHandle.insert(h, obj);
			}
		}
	}

	return true;
}

void QJDnsSharedPrivate::removeInterface(const QHostAddress &addr)
{
	Instance *i = 0;
	for(int n = 0; n < instances.count(); ++n)
	{
		if(instances[n]->addr == addr)
		{
			i = instances[n];
			break;
		}
	}
	if(!i)
		return;

	int index = i->index;

	// we don't cancel operations or shutdown jdns, we simply
	//	 delete our references.  this is because if the interface
	//	 is gone, then we have nothing to send on anyway.

	foreach(QJDnsSharedRequest *obj, requests)
	{
		for(int n = 0; n < obj->d->handles.count(); ++n)
		{
			Handle h = obj->d->handles[n];
			if(h.jdns == i->jdns)
			{
				// see above, no need to cancel the operation
				obj->d->handles.removeAt(n);
				requestForHandle.remove(h);
				break;
			}
		}

		// remove published reference
		if(obj->d->type == QJDnsSharedRequest::Publish)
		{
			for(int n = 0; n < obj->d->published.count(); ++n)
			{
				Handle h = obj->d->published[n];
				if(h.jdns == i->jdns)
				{
					obj->d->published.removeAt(n);
					break;
				}
			}
		}
	}

	// see above, no need to shutdown jdns
	instanceForQJDns.remove(i->jdns);
	instances.removeAll(i);
	delete i->jdns;
	delete i;

	// if that was the last interface to be removed, then there should
	//   be no more handles left.  let's take action with these
	//   handleless requests.
	foreach(QJDnsSharedRequest *obj, requests)
	{
		if(obj->d->handles.isEmpty())
		{
			if(mode == QJDnsShared::UnicastInternet || mode == QJDnsShared::UnicastLocal)
			{
				// for unicast, we'll invalidate with ErrorNoNet
				obj->d->success = false;
				obj->d->error = QJDnsSharedRequest::ErrorNoNet;
				obj->d->lateTimer.start();
			}
			else // Multicast
			{
				// for multicast, we'll keep all requests alive.
				//   activity will resume when an interface is
				//   added.
			}
		}
	}

	addDebug(index, QString("removing from %1").arg(addr.toString()));
}

void QJDnsSharedPrivate::queryStart(QJDnsSharedRequest *obj, const QByteArray &name, int qType)
{
	obj->d->type = QJDnsSharedRequest::Query;
	obj->d->success = false;
	obj->d->results.clear();
	obj->d->name = name;
	obj->d->qType = qType;

	// is the input an IP address and the qType is an address record?
	if(qType == QJDns::Aaaa || qType == QJDns::A)
	{
		QHostAddress addr;
		if(addr.setAddress(QString::fromLocal8Bit(name)))
		{
			if(qType == QJDns::Aaaa && addr.protocol() == QAbstractSocket::IPv6Protocol)
			{
				QJDns::Record rec;
				rec.owner = name;
				rec.type = QJDns::Aaaa;
				rec.ttl = 120;
				rec.haveKnown = true;
				rec.address = addr;
				obj->d->success = true;
				obj->d->results = QList<QJDns::Record>() << rec;
				obj->d->lateTimer.start();
				return;
			}
			else if(qType == QJDns::A && addr.protocol() == QAbstractSocket::IPv4Protocol)
			{
				QJDns::Record rec;
				rec.owner = name;
				rec.type = QJDns::A;
				rec.ttl = 120;
				rec.haveKnown = true;
				rec.address = addr;
				obj->d->success = true;
				obj->d->results = QList<QJDns::Record>() << rec;
				obj->d->lateTimer.start();
				return;
			}
		}
	}

	QJDns::SystemInfo sysInfo = get_sys_info();

	// is the input name a known host and the qType is an address record?
	if(qType == QJDns::Aaaa || qType == QJDns::A)
	{
		QByteArray lname = name.toLower();
		QList<QJDns::DnsHost> known = sysInfo.hosts;
		foreach(QJDns::DnsHost host, known)
		{
			if(((qType == QJDns::Aaaa && host.address.protocol() == QAbstractSocket::IPv6Protocol)
				|| (qType == QJDns::A && host.address.protocol() == QAbstractSocket::IPv4Protocol))
				&& host.name.toLower() == lname)
			{
				QJDns::Record rec;
				rec.owner = name;
				rec.type = qType;
				rec.ttl = 120;
				rec.haveKnown = true;
				rec.address = host.address;
				obj->d->success = true;
				obj->d->results = QList<QJDns::Record>() << rec;
				obj->d->lateTimer.start();
				return;
			}
		}
	}

	// if we have no QJDns instances to operate on, then error
	if(instances.isEmpty())
	{
		obj->d->error = QJDnsSharedRequest::ErrorNoNet;
		obj->d->lateTimer.start();
		return;
	}

	if(mode == QJDnsShared::UnicastInternet)
	{
		// get latest nameservers, split into ipv6/v4, apply to jdns instances
		QList<QJDns::NameServer> ns_v6;
		QList<QJDns::NameServer> ns_v4;
		{
			QList<QJDns::NameServer> nameServers = sysInfo.nameServers;
			foreach(QJDns::NameServer ns, nameServers)
			{
				if(ns.address.protocol() == QAbstractSocket::IPv6Protocol)
					ns_v6 += ns;
				else
					ns_v4 += ns;
			}
		}
		foreach(Instance *i, instances)
		{
			if(i->addr.protocol() == QAbstractSocket::IPv6Protocol)
				i->jdns->setNameServers(ns_v6);
			else
				i->jdns->setNameServers(ns_v4);
		}
	}

	// keep track of this request
	requests += obj;

	// query on all jdns instances
	foreach(Instance *i, instances)
	{
		Handle h(i->jdns, i->jdns->queryStart(name, qType));
		obj->d->handles += h;

		// keep track of this handle for this request
		requestForHandle.insert(h, obj);
	}
}

void QJDnsSharedPrivate::queryCancel(QJDnsSharedRequest *obj)
{
	if(!requests.contains(obj))
		return;

	foreach(Handle h, obj->d->handles)
	{
		h.jdns->queryCancel(h.id);
		requestForHandle.remove(h);
	}

	obj->d->handles.clear();
	requests.remove(obj);
}

void QJDnsSharedPrivate::publishStart(QJDnsSharedRequest *obj, QJDns::PublishMode m, const QJDns::Record &record)
{
	obj->d->type = QJDnsSharedRequest::Publish;
	obj->d->success = false;
	obj->d->results.clear();
	obj->d->pubmode = m;
	obj->d->ppmode = determinePpMode(record);
	obj->d->pubrecord = manipulateRecord(record, obj->d->ppmode);

	// if we have no QJDns instances to operate on, then error
	if(instances.isEmpty())
	{
		obj->d->error = QJDnsSharedRequest::ErrorNoNet;
		obj->d->lateTimer.start();
		return;
	}

	// keep track of this request
	requests += obj;

	// attempt to publish on all jdns instances
	foreach(QJDnsSharedPrivate::Instance *i, instances)
	{
		Handle h(i->jdns, i->jdns->publishStart(m, obj->d->pubrecord));
		obj->d->handles += h;

		// keep track of this handle for this request
		requestForHandle.insert(h, obj);
	}
}

void QJDnsSharedPrivate::publishUpdate(QJDnsSharedRequest *obj, const QJDns::Record &record)
{
	if(!requests.contains(obj))
		return;

	obj->d->ppmode = determinePpMode(record);
	obj->d->pubrecord = manipulateRecord(record, obj->d->ppmode);

	// publish update on all handles for this request
	foreach(Handle h, obj->d->handles)
		h.jdns->publishUpdate(h.id, obj->d->pubrecord);
}

void QJDnsSharedPrivate::publishCancel(QJDnsSharedRequest *obj)
{
	if(!requests.contains(obj))
		return;

	foreach(Handle h, obj->d->handles)
	{
		h.jdns->publishCancel(h.id);
		requestForHandle.remove(h);
	}

	obj->d->handles.clear();
	obj->d->published.clear();
	requests.remove(obj);
}

void QJDnsSharedPrivate::jdns_resultsReady(int id, const QJDns::Response &results)
{
	QJDns *jdns = (QJDns *)sender();
	QJDnsSharedRequest *obj = findRequest(jdns, id);
	Q_ASSERT(obj);

	obj->d->success = true;
	obj->d->results = results.answerRecords;

	if(mode == QJDnsShared::UnicastInternet || mode == QJDnsShared::UnicastLocal)
	{
		// only one response, so "cancel" it
		for(int n = 0; n < obj->d->handles.count(); ++n)
		{
			Handle h = obj->d->handles[n];
			if(h.jdns == jdns && h.id == id)
			{
				obj->d->handles.removeAt(n);
				requestForHandle.remove(h);
				break;
			}
		}

		// cancel related handles
		foreach(Handle h, obj->d->handles)
		{
			h.jdns->queryCancel(h.id);
			requestForHandle.remove(h);
		}

		obj->d->handles.clear();
		requests.remove(obj);
	}
	else // Multicast
	{
		// check our cache to see how we should report these results
		for(int n = 0; n < obj->d->results.count(); ++n)
		{
			QJDns::Record &r = obj->d->results[n];

			// do we have this answer already in our cache?
			QJDns::Record *c = 0;
			int c_at = -1;
			for(int k = 0; k < obj->d->queryCache.count(); ++k)
			{
				QJDns::Record &tmp = obj->d->queryCache[k];
				if(matchRecordExceptTtl(r, tmp))
				{
					c = &tmp;
					c_at = k;
					break;
				}
			}

			// don't report duplicates or unknown removals
			if((c && r.ttl != 0) || (!c && r.ttl == 0))
			{
				obj->d->results.removeAt(n);
				--n; // adjust position
				continue;
			}

			// if we have it, and it is removed, remove from cache
			if(c && r.ttl == 0)
			{
				obj->d->queryCache.removeAt(c_at);
			}
			// otherwise, if we don't have it, add it to the cache
			else if(!c)
			{
				obj->d->queryCache += r;
			}
		}

		if(obj->d->results.isEmpty())
			return;
	}

	emit obj->resultsReady();
}

void QJDnsSharedPrivate::jdns_published(int id)
{
	QJDns *jdns = (QJDns *)sender();
	QJDnsSharedRequest *obj = findRequest(jdns, id);
	Q_ASSERT(obj);

	// find handle
	Handle handle;
	for(int n = 0; n < obj->d->handles.count(); ++n)
	{
		Handle h = obj->d->handles[n];
		if(h.jdns == jdns && h.id == id)
		{
			handle = h;
			break;
		}
	}

	obj->d->published += handle;

	// if this publish has already been considered successful, then
	//	 a publish has succeeded on a new interface and there's no
	//	 need to report success for this request again
	if(obj->d->success)
		return;

	// all handles published?
	if(obj->d->published.count() == obj->d->handles.count())
	{
		obj->d->success = true;
		emit obj->resultsReady();
	}
}

void QJDnsSharedPrivate::jdns_error(int id, QJDns::Error e)
{
	QJDns *jdns = (QJDns *)sender();
	QJDnsSharedRequest *obj = findRequest(jdns, id);
	Q_ASSERT(obj);

	// "cancel" it
	for(int n = 0; n < obj->d->handles.count(); ++n)
	{
		Handle h = obj->d->handles[n];
		if(h.jdns == jdns && h.id == id)
		{
			obj->d->handles.removeAt(n);
			requestForHandle.remove(h);
			break;
		}
	}

	if(obj->d->type == QJDnsSharedRequest::Query)
	{
		// ignore the error if it is not the last error
		if(!obj->d->handles.isEmpty())
			return;

		requests.remove(obj);

		obj->d->success = false;
		QJDnsSharedRequest::Error x = QJDnsSharedRequest::ErrorGeneric;
		if(e == QJDns::ErrorNXDomain)
			x = QJDnsSharedRequest::ErrorNXDomain;
		else if(e == QJDns::ErrorTimeout)
			x = QJDnsSharedRequest::ErrorTimeout;
		else // ErrorGeneric
			x = QJDnsSharedRequest::ErrorGeneric;
		obj->d->error = x;
		emit obj->resultsReady();
	}
	else // Publish
	{
		// cancel related handles
		foreach(Handle h, obj->d->handles)
		{
			h.jdns->publishCancel(h.id);
			requestForHandle.remove(h);
		}

		obj->d->handles.clear();
		obj->d->published.clear();
		requests.remove(obj);

		obj->d->success = false;
		QJDnsSharedRequest::Error x = QJDnsSharedRequest::ErrorGeneric;
		if(e == QJDns::ErrorConflict)
			x = QJDnsSharedRequest::ErrorConflict;
		else // ErrorGeneric
			x = QJDnsSharedRequest::ErrorGeneric;
		obj->d->error = x;
		emit obj->resultsReady();
	}
}

void QJDnsSharedPrivate::jdns_shutdownFinished()
{
	QJDns *jdns = (QJDns *)sender();

	addDebug(instanceForQJDns.value(jdns)->index, "jdns_shutdownFinished, removing interface");

	Instance *instance = instanceForQJDns.value(jdns);
	delete instance->jdns;
	delete instance;
	instanceForQJDns.remove(jdns);
	instances.removeAll(instance);

	if(instances.isEmpty())
		late_shutdown();
}

void QJDnsSharedPrivate::jdns_debugLinesReady()
{
	QJDns *jdns = (QJDns *)sender();

	doDebug(jdns, instanceForQJDns.value(jdns)->index);
}
