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
#ifndef QPIN_H
#define QPIN_H

#include <QObject>
#include <QVariant>
#include <machinetalk/protobuf/message.pb.h>

class QHalPin : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
    Q_PROPERTY(HalPinType type READ type WRITE setType NOTIFY typeChanged)
    Q_PROPERTY(HalPinDirection direction READ direction WRITE setDirection NOTIFY directionChanged)
    Q_PROPERTY(QVariant value READ value WRITE setValue NOTIFY valueChanged)
    Q_PROPERTY(int handle READ handle NOTIFY handleChanged)
    Q_PROPERTY(bool enabled READ enabled WRITE setEnabled NOTIFY enabledChanged)
    Q_PROPERTY(bool synced READ synced NOTIFY syncedChanged)
    Q_ENUMS(HalPinType)
    Q_ENUMS(HalPinDirection)

public:
    explicit QHalPin(QObject *parent = 0);

    enum HalPinType {
        Bit = pb::HAL_BIT,
        Float = pb::HAL_FLOAT,
        S32 = pb::HAL_S32,
        U32 = pb::HAL_U32
    };

    enum HalPinDirection {
      In = pb::HAL_IN,
      Out = pb::HAL_OUT,
      IO = pb::HAL_IO
    };

    QString name() const
    {
        return m_name;
    }

    HalPinType type() const
    {
        return m_type;
    }

    HalPinDirection direction() const
    {
        return m_direction;
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

signals:

    void nameChanged(QString arg);
    void typeChanged(HalPinType arg);
    void directionChanged(HalPinDirection arg);
    void valueChanged(QVariant arg);
    void handleChanged(int arg);
    void enabledChanged(bool arg);
    void syncedChanged(bool arg);

public slots:

void setType(HalPinType arg);
void setName(QString arg);
void setDirection(HalPinDirection arg);
void setValue(QVariant arg, bool synced = false);
void setHandle(int arg);
void setEnabled(bool arg);
void setSynced(bool arg);

private:
    QString         m_name;
    HalPinType       m_type;
    HalPinDirection m_direction;
    QVariant        m_value;
    QVariant        m_syncValue;
    int             m_handle;
    bool            m_enabled;
    bool            m_synced;
};



#endif // QPIN_H
