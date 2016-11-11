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

#ifndef HALSIGNAL_H
#define HALSIGNAL_H

#include <QObject>
#include <QVariant>
#include <machinetalk/protobuf/message.pb.h>

namespace qtquickvcp {

class HalSignal : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
    Q_PROPERTY(ValueType type READ type NOTIFY typeChanged)
    Q_PROPERTY(QVariant value READ value NOTIFY valueChanged)
    Q_PROPERTY(int handle READ handle NOTIFY handleChanged)
    Q_PROPERTY(bool enabled READ enabled WRITE setEnabled NOTIFY enabledChanged)
    Q_PROPERTY(bool synced READ synced NOTIFY syncedChanged)
    Q_ENUMS(ValueType)

public:
    explicit HalSignal(QObject *parent = 0);

    enum ValueType {
        Bit = pb::HAL_BIT,
        Float = pb::HAL_FLOAT,
        S32 = pb::HAL_S32,
        U32 = pb::HAL_U32
    };

    QString name() const
    {
        return m_name;
    }

    ValueType type() const
    {
        return m_type;
    }

    QVariant value() const
    {
        return m_value;
    }

    int handle() const
    {
        return m_handle;
    }

    bool enabled() const
    {
        return m_enabled;
    }

    bool synced() const
    {
        return m_synced;
    }

public slots:
    void setName(QString arg);
    void setType(ValueType arg);
    void setValue(QVariant arg);
    void setHandle(int arg);
    void setEnabled(bool arg);
    void setSynced(bool arg);

signals:
    void nameChanged(QString arg);
    void typeChanged(ValueType arg);
    void valueChanged(QVariant arg);
    void handleChanged(int arg);
    void enabledChanged(bool arg);
    void syncedChanged(bool arg);

private:
    QString m_name;
    ValueType m_type;
    QVariant m_value;
    int m_handle;
    bool m_enabled;
    bool m_synced;
}; // class HalSignal
} // namespace qtquickvcp

#endif // HALSIGNAL_H
