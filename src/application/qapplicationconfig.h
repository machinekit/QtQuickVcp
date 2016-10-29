/****************************************************************************
**
** Copyright (C) 2014 Alexander Rössler
** License: LGPL version 2.1
**
** This file is part of QtQuickVcp.
**
** All rights reserved. This program and the accompanying materials
** are made available under the terms of the GNU Lesser General Public License
** (LGPL) version 2.1 which accompanies this distribution, and is available at
** http://www.gnu.org/licenses/lgpl-2.1.html
**
** This library is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
** Lesser General Public License for more details.
**
** Contributors:
** Alexander Rössler @ The Cool Tool GmbH <mail DOT aroessler AT gmail DOT com>
**
****************************************************************************/
#ifndef QAPPCONFIG_H
#define QAPPCONFIG_H

#include <QQuickItem>
#include <QQmlListProperty>
#include <QTimer>
#include <QCoreApplication>
#include <QDir>
#include <nzmqt/nzmqt.hpp>
#include <google/protobuf/text_format.h>
#include "qapplicationconfigitem.h"
#include "qapplicationconfigfilter.h"
#include "qapplicationdescription.h"
#include <machinetalk/protobuf/message.pb.h>
#include <machinetalk/protobuf/types.pb.h>

class QApplicationConfig : public QQuickItem
{
    Q_OBJECT
    Q_INTERFACES(QQmlParserStatus)
    Q_PROPERTY(QString configUri READ configUri WRITE setConfigUri NOTIFY configUriChanged)
    Q_PROPERTY(bool ready READ isReady WRITE setReady NOTIFY readyChanged)
    Q_PROPERTY(bool connected READ isConnected NOTIFY connectedChanged)
    Q_PROPERTY(State connectionState READ connectionState NOTIFY connectionStateChanged)
    Q_PROPERTY(ConnectionError error READ error NOTIFY errorChanged)
    Q_PROPERTY(QString errorString READ errorString NOTIFY errorStringChanged)
    Q_PROPERTY(QApplicationConfigItem *selectedConfig READ selectedConfig NOTIFY selectedConfigChanged)
    Q_PROPERTY(QQmlListProperty<QApplicationConfigItem> configs READ configs NOTIFY configsChanged)
    Q_PROPERTY(QApplicationConfigFilter *filter READ filter WRITE setFilter NOTIFY filterChanged)
    Q_ENUMS(State)
    Q_ENUMS(ConnectionError)
public:
    explicit QApplicationConfig(QQuickItem *parent = 0);
    ~QApplicationConfig();

    enum State {
        Disconnected = 0,
        Connected = 1,
        Error = 2
    };

    enum ConnectionError {
        NoError = 0,
        SocketError = 1
    };

    virtual void componentComplete();

    QString configUri() const;
    bool isReady() const;
    bool isConnected() const;
    QApplicationConfigItem *selectedConfig() const;
    QApplicationConfigFilter *filter() const;
    State connectionState() const;
    ConnectionError error() const;
    QString errorString() const;
    QQmlListProperty<QApplicationConfigItem> configs();
    int appConfigCount() const;
    QApplicationConfigItem *appConfig(int index) const;

public slots:

    void selectConfig(QString name);
    void unselectConfig();
    void setConfigUri(QString arg);
    void setReady(bool arg);
    void setSelectedConfig(QApplicationConfigItem * arg);
    void setFilter(QApplicationConfigFilter * arg);

private:
    bool    m_componentCompleted;
    QString m_configUri;
    bool    m_ready;
    bool    m_connected;
    State   m_connectionState;
    ConnectionError m_error;
    QString m_errorString;

    QApplicationConfigItem *m_selectedConfig;
    QList<QApplicationConfigItem*> m_configs;
    QApplicationConfigFilter *m_filter;

    nzmqt::PollingZMQContext *m_context;
    nzmqt::ZMQSocket *m_configSocket;
    // more efficient to reuse a protobuf Message
    pb::Container m_rx;
    pb::Container m_tx;

    void start();
    void stop();
    void updateState(State state);
    void updateError(ConnectionError error, QString errorString);
    void sendConfigMessage(const QByteArray &data);
    void cleanupFiles();

private slots:
    bool connectSocket();
    void disconnectSocket();
    void configMessageReceived(const QList<QByteArray> &messageList);
    void pollError(int errorNum, const QString &errorMsg);
    void request(pb::ContainerType type);

signals:
    void configUriChanged(QString arg);
    void readyChanged(bool arg);
    void selectedConfigChanged(QApplicationConfigItem * arg);
    void configsChanged(QQmlListProperty<QApplicationConfigItem> arg);
    void filterChanged(QApplicationConfigFilter * arg);
    void connectionStateChanged(State arg);
    void errorChanged(ConnectionError arg);
    void errorStringChanged(QString arg);
    void connectedChanged(bool arg);
};

#endif // QAPPCONFIG_H
