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
#include <machinetalk/protobuf/config.pb.h>

namespace qtquickvcp {

class ApplicationConfigItem : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
    Q_PROPERTY(QString description READ description WRITE setDescription NOTIFY descriptionChanged)
    Q_PROPERTY(ApplicationConfigItem::ApplicationType type READ type WRITE setType NOTIFY typeChanged)
    Q_PROPERTY(QUrl webUri READ webUri WRITE setWebUri NOTIFY webUriChanged)
    Q_PROPERTY(bool loaded READ isLoaded WRITE setLoaded NOTIFY loadedChanged)
    Q_PROPERTY(bool loading READ isLoading WRITE setLoading NOTIFY loadingChanged)
    Q_PROPERTY(QStringList files READ files WRITE setFiles NOTIFY filesChanged)
    Q_PROPERTY(QUrl mainFile READ mainFile WRITE setMainFile NOTIFY mainFileChanged)
    Q_PROPERTY(QUrl translationsPath READ translationsPath WRITE setTranslationsPath NOTIFY translationsPathChanged)

public:
    explicit ApplicationConfigItem(QObject *parent = 0);

    enum ApplicationType {
        Qt5QmlApplication = machinetalk::QT5_QML,
        GladeVcpApplication = machinetalk::GLADEVCP,
        JavaScriptApplication = machinetalk::JAVASCRIPT
    };
    Q_ENUM(ApplicationType)

    QString name() const;
    QString description() const;
    ApplicationType type() const;
    QUrl webUri() const;
    bool isLoaded() const;
    QStringList files() const;
    QUrl mainFile() const;
    bool isLoading() const;
    QUrl translationsPath() const;

public slots:

    void setName(const QString &arg);
    void setDescription(const QString &arg);
    void setType(ApplicationType arg);
    void setWebUri(const QUrl &arg);
    void setLoaded(bool arg);
    void setFiles(const QStringList &arg);
    void setMainFile(const QUrl &arg);
    void setLoading(bool arg);
    void setTranslationsPath(const QUrl &translationsPath);

private:
    QString m_name;
    QString m_description;
    ApplicationType m_type;
    QUrl m_webUri;
    bool m_loaded;
    bool m_loading;
    QStringList m_files;
    QUrl m_mainFile;
    QUrl m_translationsPath;

signals:

    void nameChanged(const QString &arg);
    void descriptionChanged(const QString &arg);
    void typeChanged(ApplicationType arg);
    void webUriChanged(const QUrl &arg);
    void loadedChanged(bool arg);
    void filesChanged(const QStringList &arg);
    void mainFileChanged(const QUrl &arg);
    void loadingChanged(bool arg);
    void translationsPathChanged(const QUrl &translationsPath);

}; // class ApplicationConfigItem
} // namespace qtquickvcp

#endif // APPLICATIONCONFIGITEM_H
