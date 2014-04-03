/*
 * Copyright (C) 2006,2007  Justin Karneges
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

#ifndef QJDNSSHARED_H
#define QJDNSSHARED_H

#include "qjdns.h"

class QJDnsShared;
class QJDnsSharedPrivate;
class QJDnsSharedRequestPrivate;
class QJDnsSharedDebugPrivate;

/**
   \brief Collects debugging information from QJDnsShared

   \note Iris users should utilize NetNames for DNS capabilities, <i>not</i> QJDnsSharedDebug.  See the QJDnsShared documentation for more information.

   QJDnsSharedDebug is used to collect debugging information from one or many QJDnsShared objects.  To use it, simply create it and pass it to QJDnsShared::setDebug().

   Example use:

\code
QJDnsSharedDebug *db = new QJDnsSharedDebug;
connect(db, SIGNAL(debugLinesReady(QStringList)),
	SLOT(db_debugLinesReady(QStringList)));

QJDnsShared *jdnsShared1 = new QJDnsShared(QJDnsShared::UnicastInternet);
jdnsShared1->setDebug(db, "U");

QJDnsShared *jdnsShared2 = new QJDnsShared(QJDnsShared::UnicastLocal);
jdnsShared2->setDebug(db, "L");
...
void db_debugLinesReady(const QStringList &lines)
{
	foreach(QString line, lines)
		printf("%s\n", qPrintable(line));
}
\endcode

   QJDnsShared reports debug lines with the name and interface number prepended to each line.  For example, if there is debug information to report about the second interface added to \a jdnsShared2 in the above example, the lines would be prepended with "L1: ".

   Do not destroy QJDnsSharedDebug until all of the QJDnsShared objects associated with it have been destroyed.

   \sa QJDnsShared QJDnsSharedRequest
*/
class JDNS_EXPORT QJDnsSharedDebug : public QObject
{
	Q_OBJECT
public:
	/**
	   \brief Constructs a new object with the given \a parent
	*/
	QJDnsSharedDebug(QObject *parent = 0);

	/**
	   \brief Destroys the object
	*/
	~QJDnsSharedDebug();

	/**
	   \brief Read the available debug information

	   Debug information is reported as a series of lines.  The lines are of reasonable length, and so if you're storing a backlog of the most recent debug information, it should be safe to make the cut-off point based on lines.

	   \sa readyRead
	*/
	QStringList readDebugLines();

signals:
	/**
	   \brief Emitted when there is debug information to report

	   \sa readDebugLines
	*/
	void readyRead();

private:
	friend class QJDnsShared;
	friend class QJDnsSharedPrivate;
	friend class QJDnsSharedDebugPrivate;
	QJDnsSharedDebugPrivate *d;
};

/**
   \brief Performs a DNS operation using QJDnsShared

   \note Iris users should utilize NetNames for DNS capabilities, <i>not</i> QJDnsSharedRequest.  See the QJDnsShared documentation for more information.

   QJDnsSharedRequest is used to perform DNS operations on a QJDnsShared object.  Many requests may be performed simultaneously, such that a single QJDnsShared object can be "shared" across the application.  Please see the QJDnsShared documentation for more complete information about how the overall system works.

   Call query() to perform a query.  Call publish() (or publishUpdate()) to make DNS records available on the local network (QJDnsShared::Multicast mode only).  When the operation has something to report, the resultsReady() signal is emitted.  Call success() to determine the status of the operation.  If success() returns false, then the operation has failed and the reason for the failure can be determined with error().  If success() returns true, then the meaning differs depending on the type of operation being performed:
   <ul>
     <li>For QJDnsShared::UnicastInternet and QJDnsShared::UnicastLocal modes, call results() to obtain the records obtained by the query.  In these modes, resultsReady() is only emitted once, at which point the operation is no longer active.</li>
     <li>For QJDnsShared::Multicast, operations are long-lived.  Query operations never timeout, and resultsReady() may be emitted multiple times.  In order to stop the query, either call cancel() or destroy the QJDnsSharedRequest object.  Similarly, publishing is long-lived.  The record stays published as long as the QJDnsSharedRequest has not been cancelled or destroyed.</li>
   </ul>

   Here is how you might look up an A record:

\code
QJDnsSharedRequest *req = new QJDnsSharedRequest(jdnsShared);
connect(req, SIGNAL(resultsReady()), SLOT(req_resultsReady()));
req->query("psi-im.org", QJDns::A);
...
void req_resultsReady()
{
	if(req->success())
	{
		// print all of the IP addresses obtained
		QList<QJDns::Record> results = req->results();
		foreach(QJDns::Record r, results)
		{
			if(r.type == QJDns::A)
				printf("%s\n", qPrintable(r.address.toString());
		}
	}
	else
		printf("Error resolving!\n");
}
\endcode

   Here is an example of publishing a record:

\code
QJDnsSharedRequest *pub = new QJDnsSharedRequest(jdnsShared);
connect(pub, SIGNAL(resultsReady()), SLOT(pub_resultsReady()));

// let's publish an A record
QJDns::Record rec;
rec.owner = "SomeComputer.local.";
rec.type = QJDns::A;
rec.ttl = 120;
rec.haveKnown = true;
rec.address = QHostAddress("192.168.0.32");

pub->publish(QJDns::Unique, rec);
...
void pub_resultsReady()
{
	if(pub->success())
		printf("Record published\n");
	else
		printf("Error publishing!\n");
}
\endcode

   To update an existing record, use publishUpdate():

\code
// the IP address of the host changed, so make a new record
QJDns::Record rec;
rec.owner = "SomeComputer.local.";
rec.type = QJDns::A;
rec.ttl = 120;
rec.haveKnown = true;
rec.address = QHostAddress("192.168.0.64");

// update it
pub->publishUpdate(rec);
\endcode

   As a special exception, the address value can be left unspecified for A and Aaaa record types, which tells QJDnsShared to substitute the address value with the address of whatever interfaces the record gets published on.  This is the preferred way to publish the IP address of your own machine, and in fact it is the only way to do so if you have multiple interfaces, because there will likely be a different IP address value for each interface (the record resolves to a different answer depending on which interface a query comes from).

\code
// let's publish our own A record
QJDns::Record rec;
rec.owner = "MyComputer.local.";
rec.type = QJDns::A;
rec.ttl = 120;
rec.haveKnown = true;
rec.address = QHostAddress();

pub->publish(QJDns::Unique, rec);
\endcode

   When you want to unpublish, call cancel() or destroy the QJDnsSharedRequest.

   \sa QJDnsShared
*/
class JDNS_EXPORT QJDnsSharedRequest : public QObject
{
	Q_OBJECT
public:
	/**
	   \brief Operation type
	*/
	enum Type
	{
		Query,   ///< Query operation, initiated by query()
		Publish  ///< Publish operation, initiated by publish() or publishUpdate()
	};

	/**
	   \brief Request error
	*/
	enum Error
	{
		ErrorNoNet,     ///< There are no available network interfaces to operate on.  This happens if QJDnsShared::addInterface() was not called.
		ErrorGeneric,   ///< Generic error during the operation.
		ErrorNXDomain,  ///< The name looked up does not exist.
		ErrorTimeout,   ///< The operation timed out.
		ErrorConflict   ///< Attempt to publish an already published unique record.
	};

	/**
	   \brief Constructs a new object with the given \a jdnsShared and \a parent
	*/
	QJDnsSharedRequest(QJDnsShared *jdnsShared, QObject *parent = 0);

	/**
	   \brief Destroys the object

	   If there is an active operation, it is cancelled.
	*/
	~QJDnsSharedRequest();

	/**
	   \brief The type of operation being performed
	*/
	Type type();

	/**
	   \brief Perform a query operation
	*/
	void query(const QByteArray &name, int type);

	/**
	   \brief Perform a publish operation
	*/
	void publish(QJDns::PublishMode m, const QJDns::Record &record);

	/**
	   \brief Update a record that is currently published
	*/
	void publishUpdate(const QJDns::Record &record);

	/**
	   \brief Cancels the current operation
	*/
	void cancel();

	/**
	   \brief Indicates whether or not the operation was successful
	*/
	bool success() const;

	/**
	   \brief Returns the reason for error
	*/
	Error error() const;

	/**
	   \brief Returns the results of the operation
	*/
	QList<QJDns::Record> results() const;

signals:
	/**
	   \brief Indicates that the operation has something to report

	   After receiving this signal, call success() to check on the status of the operation, followed by results() or error() as appropriate.
	*/
	void resultsReady();

private:
	friend class QJDnsShared;
	friend class QJDnsSharedPrivate;
	friend class QJDnsSharedRequestPrivate;
	QJDnsSharedRequestPrivate *d;
};

/**
   \brief Abstraction layer on top of QJDns

   \note Iris users should utilize NetNames for DNS capabilities, <i>not</i> QJDnsShared.  QJDnsShared is provided for non-Iris users (and it is also used internally by NetNames).  To use QJDnsShared by itself, simply drop the jdnsshared.h and jdnsshared.cpp files, along with JDNS, into your project.  It is not a full replacement for Qt's Q3Dns, as some tasks are left to you, but it covers most of it.

   QJDns supports everything a typical application should ever need in DNS.  However, it is expected that modern applications will need to maintain multiple QJDns instances at the same time, and this is where things can get complicated.  For example, most applications will want at least two QJDns instances: one for IPv4 unicast and one for IPv6 unicast.

   A single QJDnsShared object encapsulates multiple instances of QJDns that are related.  For example, an IPv4 unicast instance and an IPv6 unicast instance could be coupled within QJDnsShared.  Then, when a unicast operation is performed on the QJDnsShared object, both underlying instances will be queried as appropriate.  The application would not need to perform two resolutions itself, nor deal with any related complexity.

   Further, individual operations are performed using a separate class called QJDnsSharedRequest, eliminating the need for the application to directly interface with a central QJDns object or track integer handles.  This makes it easier for individual parts of the application to "share" the same instance (or set of instances) of QJDns, hence the name.

   QJDnsShared is a thin abstraction.  QJDns subtypes (e.g. QJDns::Type, QJDns::Record, etc) are still used with QJDnsShared.  Because of the duplication of documentation effort between NetNames and QJDns, there is no formal documentation for QJDns.  Users of QJDnsShared will need to read qjdns.h, although a basic explanation of the elements can be found below.

   Types:
   <table>
     <tr><td>QJDns::Type</td><td>This is a convenience enumeration for common DNS record types.  For example: A, Aaaa, Srv, etc.  The values directly map to the integer values of the DNS protocol (e.g. Srv = 33).  See qjdns.h for all of the types and values.</td></tr>
     <tr><td>QJDns::Record</td><td>This class holds a DNS record.  The main fields are <i>type</i> (integer type, probably something listed in QJDns::Type), <i>rdata</i> (QByteArray of the record value), and <i>haveKnown</i> (boolean to indicate if a decoded form of the record value is also available).  See qjdns.h for the possible known fields.  You will most-likely always work with known types.  Received records that have a type listed in QJDns::Type are guaranteed to be known and will provide a decoded value.  If you are creating a record for publishing, you will need to set <i>owner</i>, <i>ttl</i>, and <i>type</i>.  If the type to be published is listed in QJDns::Type, then you will need to set <i>haveKnown</i> to true and set the known fields as appropriate, otherwise you need to set <i>rdata</i>.  You do not need to supply an encoded form in <i>rdata</i> for known types, it can be left empty in that case.</td></tr>
     <tr><td>QJDns::PublishMode</td><td>This is for Multicast DNS, and can either be Unique or Shared.  A shared record can be published by multiple owners (for example, a "_ssh._tcp.local." PTR record might resolve to many different SSH services owned by different machines).  A unique record can only have one owner (for example, a "mycomputer.local." A record would resolve to the IP address of the machine that published it).  Attempting to publish a record on a network where a unique record is already present will result in a conflict error.</td></tr>
   </table>

   Functions:
   <table>
     <tr><td>QJDns::detectPrimaryMulticast()</td><td>Detects a multicast interface.  Pass QHostAddress::Any or QHostAddress::AnyIPv6, depending on which type of interface is desired.</td></tr>
   </table>

   To use QJDnsShared, first create an instance of it, set it up by calling addInterface() as necessary, and then use QJDnsSharedRequest to perform operations on it.

   Here is an example of how to create and set up a QJDnsShared object for typical DNS resolution:

\code
// construct
QJDnsShared *dns = new QJDnsShared(QJDnsShared::UnicastInternet);

// add IPv4 and IPv6 interfaces
dns->addInterface(QHostAddress::Any);
dns->addInterface(QHostAddress::AnyIPv6);

// at this point, the object is ready for operation
\endcode

   Perform a resolution like this:

\code
QJDnsSharedRequest *req = new QJDnsSharedRequest(dns);
connect(req, SIGNAL(resultsReady()), SLOT(req_resultsReady()));
req->query("psi-im.org", QJDns::A);
...
void req_resultsReady()
{
	if(req->success())
	{
		// print all of the IP addresses obtained
		QList<QJDns::Record> results = req->results();
		foreach(QJDns::Record r, results)
		{
			if(r.type == QJDns::A)
				printf("%s\n", qPrintable(r.address.toString());
		}
	}
	else
		printf("Error resolving!\n");
}
\endcode

   It is important to filter the results as shown in the above example.  QJDns guarantees at least one record in the results will be of the type queried for, but there may also be CNAME records present (of course, if the query was for a CNAME type, then the results will only be CNAME records).  The recommended approach is to simply filter for the record types desired, as shown, rather than single out CNAME specifically.

   When you are finished with a QJDnsShared object, it should be shut down before deleting:

\code
connect(dns, SIGNAL(shutdownFinished()), SLOT(dns_shutdownFinished()));
dns->shutdown();
...
void dns_shutdownFinished()
{
	delete dns;
}
\endcode

   Setting up QJDnsShared for UnicastLocal and Multicast mode is done the same way as with UnicastInternet.

   For example, here is how Multicast mode could be set up:

\code
// construct
QJDnsShared *dns = new QJDnsShared(QJDnsShared::Multicast);

// add IPv4 interface
QHostAddress addr = QJDns::detectPrimaryMulticast(QHostAddress::Any);
dns->addInterface(addr);

// at this point, the object is ready for operation
\endcode

   QJDnsShared provides a lot of functionality, but certain aspects of DNS are deemed out of its scope.  Below are the responsibilities of the user of QJDnsShared, if a more complete DNS behavior is desired:
   <ul>
     <li>Querying for several "qualified" names.  You should first query for the name as provided, and if that fails then query for name + ".domain" (for every domain the computer is in).  See domains().</li>
     <li>Detecting for ".local" in the name to be queried, and using that to decide whether to query via Multicast/UnicastLocal or UnicastInternet.</li>
     <li>For zeroconf/Bonjour, keep in mind that QJDnsShared only provides low-level record queries.  DNS-SD and any higher layers would be your job.</li>
   </ul>

   Using a custom DNS implementation, such as QJDnsShared, has the drawback that it is difficult to take advantage of platform-specific features (for example, an OS-wide DNS cache or LDAP integration).  An application strategy for normal DNS should probably be:
   <ul>
     <li>If an A or AAAA record is desired, use a native lookup.</li>
     <li>Else, if the platform has advanced DNS features already (ie, res_query), use those.</li>
     <li>Else, use QJDnsShared.</li>
   </ul>

   For Multicast DNS, awareness of the platform is doubly important.  There should only be one Multicast DNS "Responder" per computer, and using QJDnsShared in Multicast mode at the same time could result in a conflict.  An application strategy for Multicast DNS should be:
   <ul>
     <li>If the platform has a Multicast DNS daemon installed already, use it somehow.</li>
     <li>Else, use QJDnsShared.</li>
   </ul>

   \sa QJDnsSharedRequest
*/
class JDNS_EXPORT QJDnsShared : public QObject
{
	Q_OBJECT
public:
	/**
	   \brief The mode to operate in
	*/
	enum Mode
	{
		/**
		   For regular DNS resolution.  In this mode, lookups are performed on all interfaces, and the first returned result is used.
		*/
		UnicastInternet,

		/**
		   Perform regular DNS resolution using the Multicast DNS address.  This is used to resolve large and/or known Multicast DNS names without actually multicasting anything.
		*/
		UnicastLocal,

		/**
		   Multicast DNS querying and publishing.

		   \note For Multicast mode, QJDnsShared supports up to one interface for each IP version (e.g. one IPv4 interface and one IPv6 interface), and expects the default/primary multicast interface for that IP version to be used.
		*/
		Multicast
	};

	/**
	   \brief Constructs a new object with the given \a mode and \a parent
	*/
	QJDnsShared(Mode mode, QObject *parent = 0);

	/**
	   \brief Destroys the object
	*/
	~QJDnsShared();

	/**
	   \brief Sets the debug object to report to

	   If a debug object is set using this function, then QJDnsShared will send output text to it, prefixing each line with \a name.
	*/
	void setDebug(QJDnsSharedDebug *db, const QString &name);

	/**
	   \brief Adds an interface to operate on

	   For UnicastInternet and UnicastLocal, these will almost always be QHostAddress::Any or QHostAddress::AnyIPv6 (operate on the default interface for IPv4 or IPv6, respectively).

	   For Multicast, it is expected that the default/primary multicast interface will be used here.  Do not pass QHostAddress::Any (or AnyIPv6) with Multicast mode.

	   Returns true if the interface was successfully added, otherwise returns false.
	*/
	bool addInterface(const QHostAddress &addr);

	/**
	   \brief Removes a previously-added interface
	*/
	void removeInterface(const QHostAddress &addr);

	/**
	   \brief Shuts down the object

	   This operation primarily exists for Multicast mode, so that any published records have a chance to be unpublished.  If the QJDnsShared object is simply deleted without performing a shutdown, then published records will linger on the network until their TTLs expire.

	   When shutdown is complete, the shutdownFinished() signal will be emitted.
	*/
	void shutdown();

	/**
	   \brief The domains to search in

	   You should perform a separate resolution for every domain configured on this machine.
	*/
	static QList<QByteArray> domains();

	/**
	   \brief Performs a blocking shutdown of many QJDnsShared instances

	   This function is a convenient way to shutdown multiple QJDnsShared instances synchronously.  The internal shutdown procedure uses no more than a few cycles of the eventloop, so it should be safe to call without worry of the application being overly stalled.  This function takes ownership of the instances passed to it, and will delete them upon completion.

	   It is worth noting that this function is implemented without the use of a nested eventloop.  All of the QJDnsShared instances are moved into a temporary thread to perform the shutdown procedure, which should not cause any unexpected behavior in the current thread.

	   \code
QList<QJDnsShared*> list;
list += jdnsShared_unicast;
list += jdnsShared_multicast;
QJDnsShared::waitForShutdown(list);

// collect remaining debug information
QStringList finalDebugLines = jdnsSharedDebug.readDebugLines();
	   \endcode
	*/
	static void waitForShutdown(const QList<QJDnsShared*> &instances);

signals:
	/**
	   \brief Indicates the object has been shut down
	*/
	void shutdownFinished();

private:
	friend class QJDnsSharedRequest;
	friend class QJDnsSharedPrivate;
	QJDnsSharedPrivate *d;
};

#endif
