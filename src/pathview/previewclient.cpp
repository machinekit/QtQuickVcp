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

#include "previewclient.h"
#include "debughelper.h"

using namespace machinetalk;

namespace qtquickvcp {

PreviewClient::PreviewClient(QObject *parent) :
    pathview::PreviewClientBase(parent),
    m_connected(false),
    m_model(nullptr),
    m_interpreterState(InterpreterStateUnset),
    m_interpreterNote(""),
    m_previewUpdated(false)
{
    m_previewStatus.fileName = "test.ngc";
    m_previewStatus.lineNumber = 0;

    addPreviewTopic("preview");
    addPreviewstatusTopic("status");
}

void PreviewClient::previewReceived(const QByteArray &topic, const Container &rx)
{
    Q_UNUSED(topic);

    if (m_model == nullptr)
    {
        return;
    }

    for (int i = 0; i < rx.preview_size(); ++i)
    {
        Preview preview;
        QModelIndex index;

        preview = rx.preview(i);

        if (preview.has_line_number())
        {
            m_previewStatus.lineNumber = qMax(preview.line_number(), 1); // line number 0 == at beginning of file
        }

        if (preview.has_filename())
        {
            m_previewStatus.fileName = QString::fromStdString(preview.filename());
        }

        if (preview.type() == PV_PREVIEW_START)
        {
            m_previewUpdated = false;
            m_model->beginUpdate();
            m_model->clearPreview(false);
            continue;
        }

        if (preview.type() == PV_PREVIEW_END)
        {
            m_model->endUpdate();
            continue;
        }

        index = m_model->index(m_previewStatus.fileName, m_previewStatus.lineNumber);
        m_model->addPreviewItem(index, preview);

        m_previewUpdated = true;
    }
}

void PreviewClient::interpStatReceived(const QByteArray &topic, const Container &rx)
{
    Q_UNUSED(topic);
    QStringList notes;

    m_interpreterState = (InterpreterState)rx.interp_state();
    for (int i = 0; i< rx.note_size(); ++i)
    {
        notes.append(QString::fromStdString(rx.note(i)));
    }
    m_interpreterNote = notes.join("\n");

    emit interpreterNoteChanged(m_interpreterNote);
    emit interpreterStateChanged(m_interpreterState);
}

void PreviewClient::setConnected()
{
    m_connected = true;
    emit connectedChanged(m_connected);
}

void PreviewClient::clearConnected()
{
    m_connected = false;
    emit connectedChanged(m_connected);
}

}; // namespace qtquickvcp
