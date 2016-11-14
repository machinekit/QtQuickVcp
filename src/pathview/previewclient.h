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

#ifndef PREVIEWCLIENT_H
#define PREVIEWCLIENT_H

#include <QObject>
#include <google/protobuf/text_format.h>
#include <machinetalk/protobuf/message.pb.h>
#include <pathview/previewclientbase.h>
#include "gcodeprogrammodel.h"

namespace qtquickvcp {

class PreviewClient : public pathview::PreviewClientBase
{
    Q_OBJECT
    Q_PROPERTY(bool connected READ isConnected NOTIFY connectedChanged)
    Q_PROPERTY(GCodeProgramModel *model READ model WRITE setModel NOTIFY modelChanged)
    Q_PROPERTY(InterpreterState interpreterState READ interpreterState NOTIFY interpreterStateChanged)
    Q_PROPERTY(QString interpreterNote READ interpreterNote NOTIFY interpreterNoteChanged)

public:
    explicit PreviewClient(QObject *parent = 0);

    enum InterpreterState {
        InterpreterIdle = pb::INTERP_IDLE,
        InterpreterRunning = pb::INTERP_RUNNING,
        InterpreterPaused = pb::INTERP_PAUSED,
        InterpreterQueueWait = pb::INTERP_QUEUE_WAIT,
        InterpreterSyncWait = pb::INTERP_SYNC_WAIT,
        InterpreterAbortWait = pb::INTERP_ABORT_WAIT,
        InterpreterStateUnset = pb::INTERP_STATE_UNSET
    };

    enum CanonUnits {
        CanonUnitsInch = pb::CANON_UNITS_INCH,
        CanonUnitsMm = pb::CANON_UNITS_MM,
        CanonUnitsCm = pb::CANON_UNITS_CM
    };

    GCodeProgramModel * model() const
    {
        return m_model;
    }

    InterpreterState interpreterState() const
    {
        return m_interpreterState;
    }

    QString interpreterNote() const
    {
        return m_interpreterNote;
    }

    bool isConnected() const
    {
        return m_connected;
    }

public slots:
    void setModel(GCodeProgramModel * arg)
    {
        if (m_model != arg) {
            m_model = arg;
            emit modelChanged(arg);
        }
    }

private:
    typedef struct {
        QString fileName;
        int lineNumber;
    } PreviewStatus;

    bool m_connected;
    GCodeProgramModel *m_model;
    InterpreterState m_interpreterState;
    QString m_interpreterNote;

    PreviewStatus m_previewStatus;
    bool m_previewUpdated;

    void previewReceived(const QByteArray &topic, const pb::Container &rx);
    void interpStatReceived(const QByteArray &topic, const pb::Container &rx);
    void setConnected();
    void clearConnected();

signals:
    void modelChanged(GCodeProgramModel * arg);
    void interpreterStateChanged(InterpreterState arg);
    void interpreterNoteChanged(QString arg);
    void connectedChanged(bool arg);
}; // class PreviewClient
} // namespace qtquickvcp

#endif // PREVIEWCLIENT_H
