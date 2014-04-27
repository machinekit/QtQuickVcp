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

#ifndef QJDNSSHARED_P_H
#define QJDNSSHARED_P_H

// SafeTimer
#include "qjdns_p.h"
#include "qjdnsshared.h"
#include <QWaitCondition>
#include <QThread>
#include <QObject>
#include <QMutex>
#include <QStringList>
#include <QSet>
#include <QHash>

class JDnsShutdownAgent : public QObject
{
	Q_OBJECT
public:
	void start();

signals:
	void started();
};

class JDnsShutdownWorker : public QObject
{
	Q_OBJECT
public:
	QList<QJDnsShared*> list;

	JDnsShutdownWorker(const QList<QJDnsShared*> &_list);

signals:
	void finished();

private slots:
	void jdns_shutdownFinished();
};

class JDnsShutdown : public QThread
{
	Q_OBJECT
public:
	QMutex m;
	QWaitCondition w;
	QList<QJDnsShared*> list;
	JDnsShutdownAgent *agent;
	JDnsShutdownWorker *worker;
	int phase;

	void waitForShutdown(const QList<QJDnsShared*> &_list);

protected:
	virtual void run();

private slots:
	void agent_started();
	void worker_finished();
};

class QJDnsSharedDebugPrivate : public QObject
{
	Q_OBJECT
public:
	QJDnsSharedDebug *q;
	QMutex m;
	QStringList lines;
	bool dirty;

	QJDnsSharedDebugPrivate(QJDnsSharedDebug *_q);
	void addDebug(const QString &name, const QStringList &_lines);

private slots:
	void doUpdate();
};

//----------------------------------------------------------------------------
// Handle
//----------------------------------------------------------------------------

// QJDns uses integer handle ids, but they are only unique within
//   the relevant QJDns instance.  Since we want our handles to be
//   unique across all instances, we'll make an instance/id pair.
class Handle
{
public:
	QJDns *jdns;
	int id;

	Handle() : jdns(0), id(-1)
	{
	}

	Handle(QJDns *_jdns, int _id) : jdns(_jdns), id(_id)
	{
	}

	bool operator==(const Handle &a) const
	{
		if(a.jdns == jdns && a.id == id)
			return true;
		return false;
	}

	bool operator!=(const Handle &a) const
	{
		return !(operator==(a));
	}
};

class QJDnsSharedPrivate : public QObject
{
	Q_OBJECT
public:
	class Instance
	{
	public:
		QJDns *jdns;
		QHostAddress addr;
		int index;

		Instance() : jdns(0)
		{
		}
	};

	enum PreprocessMode
	{
		None,			 // don't muck with anything
		FillInAddress,	 // for A/AAAA
		FillInPtrOwner6, // for PTR, IPv6
		FillInPtrOwner4, // for PTR, IPv4
	};

	QJDnsShared *q;
	QJDnsShared::Mode mode;
	bool shutting_down;
	QJDnsSharedDebug *db;
	QString dbname;

	QList<Instance*> instances;
	QHash<QJDns*,Instance*> instanceForQJDns;

	QSet<QJDnsSharedRequest*> requests;
	QHash<Handle,QJDnsSharedRequest*> requestForHandle;

	QJDnsSharedPrivate(QJDnsShared *_q);
	QJDnsSharedRequest *findRequest(QJDns *jdns, int id) const;
	void jdns_link(QJDns *jdns);
	int getNewIndex() const;
	void addDebug(int index, const QString &line);
	void doDebug(QJDns *jdns, int index);
	PreprocessMode determinePpMode(const QJDns::Record &in);
	QJDns::Record manipulateRecord(const QJDns::Record &in, PreprocessMode ppmode, bool *modified = 0);
	bool addInterface(const QHostAddress &addr);
	void removeInterface(const QHostAddress &addr);

	void queryStart(QJDnsSharedRequest *obj, const QByteArray &name, int qType);
	void queryCancel(QJDnsSharedRequest *obj);
	void publishStart(QJDnsSharedRequest *obj, QJDns::PublishMode m, const QJDns::Record &record);
	void publishUpdate(QJDnsSharedRequest *obj, const QJDns::Record &record);
	void publishCancel(QJDnsSharedRequest *obj);

public slots:
	void late_shutdown();

private slots:
	void jdns_resultsReady(int id, const QJDns::Response &results);
	void jdns_published(int id);
	void jdns_error(int id, QJDns::Error e);
	void jdns_shutdownFinished();
	void jdns_debugLinesReady();
};

class QJDnsSharedRequestPrivate : public QObject
{
	Q_OBJECT
public:
	QJDnsSharedRequest *q;
	QJDnsSharedPrivate *jsp;

	// current action
	QJDnsSharedRequest::Type type;
	QByteArray name;
	int qType;
	QJDns::PublishMode pubmode;
	QJDnsSharedPrivate::PreprocessMode ppmode;
	QJDns::Record pubrecord;

	// a single request might have to perform multiple QJDns operations
	QList<Handle> handles;

	// keep a list of handles that successfully publish
	QList<Handle> published;

	// use to weed out dups for multicast
	QList<QJDns::Record> queryCache;

	bool success;
	QJDnsSharedRequest::Error error;
	QList<QJDns::Record> results;
	SafeTimer lateTimer;
	QJDnsSharedRequestPrivate(QJDnsSharedRequest *_q);
	void resetSession();

private slots:
	void lateTimer_timeout();
};

#endif
