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

#ifndef ABSTRACTSERVICEIMPLEMENTATION_H
#define ABSTRACTSERVICEIMPLEMENTATION_H

#include <QObject>
#include <QQmlParserStatus>

namespace qtquickvcp {

class AbstractServiceImplementation : public QObject, public QQmlParserStatus
{
    Q_OBJECT
    Q_INTERFACES(QQmlParserStatus)
    Q_PROPERTY(bool ready READ ready WRITE setReady NOTIFY readyChanged)
    Q_PROPERTY(bool required READ required WRITE setRequired NOTIFY requiredChanged)

public:
    explicit AbstractServiceImplementation(QObject *parent = 0);
    ~AbstractServiceImplementation();

    void classBegin() {}
    void componentComplete();

    bool isComponentCompleted() {
        return m_componentCompleted;
    }

    bool required() const
    {
        return m_required;
    }

    bool ready() const
    {
        return m_ready;
    }

public slots:

    void setRequired(bool arg)
    {
        if (m_required == arg)
            return;

        m_required = arg;
        emit requiredChanged(arg);
    }

    void setReady(bool arg);
    void reset();

private:
    bool m_componentCompleted;
    bool m_required;
    bool m_ready;

    virtual void start() = 0;
    virtual void stop() = 0;
    virtual void cleanup() {}

private slots:

signals:
    void requiredChanged(bool arg);
    void readyChanged(bool arg);
}; // class AbstractServiceImplementation
} // namespace qtquickvcp

#endif // ABSTRACTSERVICEIMPLEMENTATION_H
