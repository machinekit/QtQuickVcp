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

#include "qpreviewclient.h"
#include "debughelper.h"

#if defined(Q_OS_IOS)
namespace gpb = google_public::protobuf;
#else
namespace gpb = google::protobuf;
#endif

using namespace nzmqt;

QPreviewClient::QPreviewClient(QObject *parent) :
    AbstractServiceImplementation(parent),
    m_statusUri(""),
    m_previewUri(""),
    m_connectionState(Disconnected),
    m_connected(false),
    m_error(NoError),
    m_errorString(""),
    m_model(nullptr),
    m_interpreterState(InterpreterStateUnset),
    m_interpreterNote(""),
    m_context(nullptr),
    m_statusSocket(nullptr),
    m_previewSocket(nullptr),
    m_previewUpdated(false)
{
    m_previewStatus.fileName = "test.ngc";
    m_previewStatus.lineNumber = 0;
}

void QPreviewClient::start()
{
#ifdef QT_DEBUG
    DEBUG_TAG(1, "preview", "start")
        #endif
            updateState(Connecting);

    if (connectSockets())
    {
        m_statusSocket->subscribeTo("status");
        m_previewSocket->subscribeTo("preview");
        updateState(Connected); //TODO: add something like a ping
    }
}

void QPreviewClient::stop()
{
#ifdef QT_DEBUG
    DEBUG_TAG(1, "preview", "stop")
#endif

    cleanup();
    updateState(Disconnected);  // clears the error
}

void QPreviewClient::cleanup()
{
    disconnectSockets();
}

void QPreviewClient::updateState(QPreviewClient::State state)
{
    updateState(state, NoError, "");
}

void QPreviewClient::updateState(QPreviewClient::State state, QPreviewClient::ConnectionError error, QString errorString)
{
    if (state != m_connectionState)
    {
        if (m_connected != (state == Connected)) {
            m_connected = (state == Connected);
            emit connectedChanged(m_connected);
        }

        m_connectionState = state;
        emit connectionStateChanged(m_connectionState);
    }

    updateError(error, errorString);
}

void QPreviewClient::updateError(QPreviewClient::ConnectionError error, QString errorString)
{
    if (m_errorString != errorString)
    {
        m_errorString = errorString;
        emit errorStringChanged(m_errorString);
    }

    if (m_error != error)
    {
        if (error != NoError)
        {
            cleanup();
        }
        m_error = error;
        emit errorChanged(m_error);
    }
}

/** Processes all message received on the status 0MQ socket */
void QPreviewClient::statusMessageReceived(QList<QByteArray> messageList)
{
    QByteArray topic;

    topic = messageList.at(0);
    m_rx.ParseFromArray(messageList.at(1).data(), messageList.at(1).size());

    #ifdef QT_DEBUG
        std::string s;
        gpb::TextFormat::PrintToString(m_rx, &s);
        DEBUG_TAG(3, "preview", "status update" << topic << QString::fromStdString(s))
    #endif

    if (m_rx.type() == pb::MT_INTERP_STAT)
    {
        QStringList notes;

        m_interpreterState = (InterpreterState)m_rx.interp_state();
        for (int i = 0; i< m_rx.note_size(); ++i)
        {
            notes.append(QString::fromStdString(m_rx.note(i)));
        }
        m_interpreterNote = notes.join("\n");

        emit interpreterNoteChanged(m_interpreterNote);
        emit interpreterStateChanged(m_interpreterState);
    }
}

/** Processes all message received on the preview 0MQ socket */
void QPreviewClient::previewMessageReceived(QList<QByteArray> messageList)
{
    QByteArray topic;

    topic = messageList.at(0);
    m_rx.ParseFromArray(messageList.at(1).data(), messageList.at(1).size());

    #ifdef QT_DEBUG
        std::string s;
        gpb::TextFormat::PrintToString(m_rx, &s);
        DEBUG_TAG(3, "preview", "preview update" << topic << QString::fromStdString(s))
    #endif

    if (m_rx.type() == pb::MT_PREVIEW)
    {
        if (m_model == nullptr)
        {
            return;
        }

        for (int i = 0; i < m_rx.preview_size(); ++i)
        {
            pb::Preview preview;
            QModelIndex index;

            preview = m_rx.preview(i);

            if (preview.has_line_number())
            {
                m_previewStatus.lineNumber = qMax(preview.line_number(), 1); // line number 0 == at beginning of file
            }

            if (preview.has_filename())
            {
                m_previewStatus.fileName = QString::fromStdString(preview.filename());
            }

            if (preview.type() == pb::PV_PREVIEW_START)
            {
                m_previewUpdated = false;
                m_model->beginUpdate();
                m_model->clearPreview(false);
                continue;
            }

            if (preview.type() == pb::PV_PREVIEW_END)
            {
                m_model->endUpdate();
                continue;
            }

            index = m_model->index(m_previewStatus.fileName, m_previewStatus.lineNumber);
            m_model->addPreviewItem(index, preview);

            m_previewUpdated = true;
        }
    }
}

void QPreviewClient::pollError(int errorNum, const QString &errorMsg)
{
    QString errorString;
    errorString = QString("Error %1: ").arg(errorNum) + errorMsg;
    updateState(Error, SocketError, errorString);
}

/** Connects the 0MQ sockets */
bool QPreviewClient::connectSockets()
{
    m_context = new PollingZMQContext(this, 1);
    connect(m_context, &PollingZMQContext::pollError,
            this, &QPreviewClient::pollError);
    m_context->start();

    m_statusSocket = m_context->createSocket(ZMQSocket::TYP_SUB, this);
    m_statusSocket->setLinger(0);

    m_previewSocket = m_context->createSocket(ZMQSocket::TYP_SUB, this);
    m_previewSocket->setLinger(0);

    try {
        m_statusSocket->connectTo(m_statusUri);
        m_previewSocket->connectTo(m_previewUri);
    }
    catch (const zmq::error_t &e) {
        QString errorString;
        errorString = QString("Error %1: ").arg(e.num()) + QString(e.what());
        updateState(Error, SocketError, errorString);
        return false;
    }

    connect(m_statusSocket, &ZMQSocket::messageReceived,
            this, &QPreviewClient::statusMessageReceived);
    connect(m_previewSocket, &ZMQSocket::messageReceived,
            this, &QPreviewClient::previewMessageReceived);

#ifdef QT_DEBUG
    DEBUG_TAG(1, "preview", "sockets connected" << m_statusUri << m_previewUri)
#endif

    return true;
}

/** Disconnects the 0MQ sockets */
void QPreviewClient::disconnectSockets()
{
    if (m_statusSocket != nullptr)
    {
        m_statusSocket->close();
        m_statusSocket->deleteLater();
        m_statusSocket = nullptr;
    }

    if (m_previewSocket != nullptr)
    {
        m_previewSocket->close();
        m_previewSocket->deleteLater();
        m_previewSocket = nullptr;
    }

    if (m_context != nullptr)
    {
        m_context->stop();
        m_context->deleteLater();
        m_context = nullptr;
    }
}
