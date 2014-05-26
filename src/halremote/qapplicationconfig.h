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
#include "qapplicationconfigitem.h"
#include "qapplicationconfigfilter.h"
#include "message.pb.h"
#include "types.pb.h"
#include <google/protobuf/text_format.h>

#if defined(Q_OS_IOS)
namespace gpb = google_public::protobuf;
#else
namespace gpb = google::protobuf;
#endif

using namespace nzmqt;

class QApplicationConfig : public QQuickItem
{
    Q_OBJECT
    Q_INTERFACES(QQmlParserStatus)
    Q_PROPERTY(QString uri READ uri WRITE setUri NOTIFY uriChanged)
    Q_PROPERTY(bool ready READ isReady WRITE setReady NOTIFY readyChanged)
    Q_PROPERTY(QApplicationConfigItem *selectedConfig READ selectedConfig NOTIFY selectedConfigChanged)
    Q_PROPERTY(QQmlListProperty<QApplicationConfigItem> appConfigs READ appConfigs NOTIFY appConfigsChanged)
    Q_PROPERTY(QQmlListProperty<QApplicationConfigFilter> filters READ filters)
public:
    explicit QApplicationConfig(QQuickItem *parent = 0);
    ~QApplicationConfig();

    virtual void componentComplete();

    QString uri() const
    {
        return m_uri;
    }

    bool isReady() const
    {
        return m_ready;
    }

    QApplicationConfigItem * selectedConfig() const
    {
        return m_selectedConfig;
    }

    QQmlListProperty<QApplicationConfigItem> appConfigs();
    int appConfigCount() const;
    QApplicationConfigItem *appConfig(int index) const;

    QQmlListProperty<QApplicationConfigFilter> filters();
    int filterCount() const;
    QApplicationConfigFilter* filter(int index) const;

public slots:

    void selectApplicationConfig(QString name);
    void unselectApplicationConfig();

    void setUri(QString arg)
    {
        if (m_uri != arg) {
            m_uri = arg;
            emit uriChanged(arg);
        }
    }

    void setReady(bool arg);

    void setSelectedConfig(QApplicationConfigItem * arg)
    {
        if (m_selectedConfig != arg) {
            m_selectedConfig = arg;
            emit selectedConfigChanged(arg);
        }
    }

private:
    QString m_uri;
    bool m_ready;
    QApplicationConfigItem * m_selectedConfig;
    QList<QApplicationConfigItem*> m_appConfigs;
    QList<QApplicationConfigFilter*> m_filters;

    ZMQContext *m_context;
    ZMQSocket *m_configSocket;
    // more efficient to reuse a protobuf Message
    pb::Container m_rx;
    pb::Container m_tx;

    bool m_componentCompleted;

    void start();
    void stop();

private slots:
    void connectSocket();
    void disconnectSocket();
    void configMessageReceived(QList<QByteArray> messageList);
    void request(pb::ContainerType type);

signals:
    void uriChanged(QString arg);
    void readyChanged(bool arg);
    void selectedConfigChanged(QApplicationConfigItem * arg);
    void appConfigsChanged(QQmlListProperty<QApplicationConfigItem> arg);
};

#endif // QAPPCONFIG_H
