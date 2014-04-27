/*
 * Copyright (C) 2005-2008  Justin Karneges
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

#ifndef QJDNS_P_H
#define QJDNS_P_H

#include "jdns.h"
#include "qjdns.h"
#include <QObject>
#include <QTime>
#include <QStringList>
#include <QHash>

class QUdpSocket;
class QTimer;

class SafeTimer : public QObject
{
	Q_OBJECT
public:
	SafeTimer(QObject *parent = 0);
	~SafeTimer();

	int interval() const;
	bool isActive() const;
	bool isSingleShot() const;
	void setInterval(int msec);
	void setSingleShot(bool singleShot);
	int timerId() const;

public slots:
	void start(int msec);
	void start();
	void stop();

signals:
	void timeout();

private:
	QTimer *t;
};

class QJDns::Private : public QObject
{
	Q_OBJECT
public:
	class LateError
	{
	public:
		int source_type; // 0 for query, 1 for publish
		int id;
		Error error;
	};

	class LateResponse
	{
	public:
		int id;
		QJDns::Response response;
		bool do_cancel;
	};

	QJDns *q;
	QJDns::Mode mode;
	jdns_session_t *sess;
	bool shutting_down;
	SafeTimer stepTrigger, debugTrigger;
	SafeTimer stepTimeout;
	QTime clock;
	QStringList debug_strings;
	bool new_debug_strings;
	int next_handle;
	bool need_handle;
	QHash<int,QUdpSocket*> socketForHandle;
	QHash<QUdpSocket*,int> handleForSocket;
	int pending;
	bool pending_wait;
	bool complete_shutdown;

	// pointers that will point to things we are currently signalling
	//   about.  when a query or publish is cancelled, we can use these
	//   pointers to extract anything we shouldn't signal.
	QList<LateError> *pErrors;
	QList<int> *pPublished;
	QList<LateResponse> *pResponses;

	Private(QJDns *_q);
	~Private();
	void cleanup();
	bool init(QJDns::Mode _mode, const QHostAddress &address);
	void setNameServers(const QList<NameServer> &nslist);
	void process();
	void processDebug();
	void doNextStep();
	void removeCancelled(int id);
	
private slots:
	void udp_readyRead();
	void udp_bytesWritten(qint64);
	void st_timeout();
	void doNextStepSlot();
	void doDebug();
	
private:
	static int cb_time_now(jdns_session_t *, void *app);
	static int cb_rand_int(jdns_session_t *, void *);
	static void cb_debug_line(jdns_session_t *, void *app, const char *str);
	static int cb_udp_bind(jdns_session_t *, void *app, const jdns_address_t *addr, int port, const jdns_address_t *maddr);
	static void cb_udp_unbind(jdns_session_t *, void *app, int handle);
	static int cb_udp_read(jdns_session_t *, void *app, int handle, jdns_address_t *addr, int *port, unsigned char *buf, int *bufsize);
	static int cb_udp_write(jdns_session_t *, void *app, int handle, const jdns_address_t *addr, int port, unsigned char *buf, int bufsize);
};

#endif
