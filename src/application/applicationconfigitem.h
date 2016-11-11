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
#ifndef APPLICATIONCONFIGITEM_H
#define APPLICATIONCONFIGITEM_H

#include <QObject>
#include <QUrl>
#include <QStringList>
#include <machinetalk/protobuf/message.pb.h>
#include <machinetalk/protobuf/types.pb.h>
#include <machinetalk/protobuf/config.pb.h>

namespace qtquickvcp {

class ApplicationConfigItem : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
    Q_PROPERTY(QString description READ description WRITE setDescription NOTIFY descriptionChanged)
    Q_PROPERTY(ApplicationType type READ type WRITE setType NOTIFY typeChanged)
    Q_PROPERTY(QUrl webUri READ webUri WRITE setWebUri NOTIFY webUriChanged)
    Q_PROPERTY(bool loaded READ isLoaded WRITE setLoaded NOTIFY loadedChanged)
    Q_PROPERTY(bool loading READ isLoading WRITE setLoading NOTIFY loadingChanged)
    Q_PROPERTY(QStringList files READ files WRITE setFiles NOTIFY filesChanged)
    Q_PROPERTY(QUrl mainFile READ mainFile WRITE setMainFile NOTIFY mainFileChanged)
    Q_ENUMS(ApplicationType)

public:
    explicit ApplicationConfigItem(QObject *parent = 0);

    enum ApplicationType {
        Qt5QmlApplication = pb::QT5_QML,
        GladeVcpApplication = pb::GLADEVCP,
        JavaScriptApplication = pb::JAVASCRIPT
    };

    QString name() const
    {
        return m_name;
    }

    QString description() const
    {
        return m_description;
    }

    ApplicationType type() const
    {
        return m_type;
    }

    QUrl webUri() const
    {
        return m_webUri;
    }

    bool isLoaded() const
    {
        return m_loaded;
    }

    QStringList files() const
    {
        return m_files;
    }

    QUrl mainFile() const
    {
        return m_mainFile;
    }

    bool isLoading() const
    {
        return m_loading;
    }

public slots:

    void setName(QString arg)
    {
        if (m_name != arg) {
            m_name = arg;
            emit nameChanged(arg);
        }
    }
    void setDescription(QString arg)
    {
        if (m_description != arg) {
            m_description = arg;
            emit descriptionChanged(arg);
        }
    }
    void setType(ApplicationType arg)
    {
        if (m_type != arg) {
            m_type = arg;
            emit typeChanged(arg);
        }
    }
    void setWebUri(QUrl arg)
    {
        if (m_webUri != arg) {
            m_webUri = arg;
            emit webUriChanged(arg);
        }
    }
    void setLoaded(bool arg)
    {
        if (m_loaded != arg) {
            m_loaded = arg;
            emit loadedChanged(arg);
        }
    }
    void setFiles(QStringList arg)
    {
        if (m_files != arg) {
            m_files = arg;
            emit filesChanged(arg);
        }
    }
    void setMainFile(QUrl arg)
    {
        if (m_mainFile != arg) {
            m_mainFile = arg;
            emit mainFileChanged(arg);
        }
    }

    void setLoading(bool arg)
    {
        if (m_loading == arg)
            return;

        m_loading = arg;
        emit loadingChanged(arg);
    }

private:
    QString m_name;
    QString m_description;
    ApplicationType m_type;
    QUrl m_webUri;
    bool m_loaded;
    bool m_loading;
    QStringList m_files;
    QUrl m_mainFile;

signals:

    void nameChanged(QString arg);

    void descriptionChanged(QString arg);

    void typeChanged(ApplicationType arg);

    void webUriChanged(QUrl arg);

    void loadedChanged(bool arg);

    void filesChanged(QStringList arg);

    void mainFileChanged(QUrl arg);


    void loadingChanged(bool arg);
}; // class ApplicationConfigItem
} // namespace qtquickvcp

#endif // APPLICATIONCONFIGITEM_H
