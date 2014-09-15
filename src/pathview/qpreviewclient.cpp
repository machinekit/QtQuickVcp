#include "qpreviewclient.h"
#include "debughelper.h"

QPreviewClient::QPreviewClient(QQuickItem *parent) :
    QQuickItem(parent),
    m_statusUri(""),
    m_previewUri(""),
    m_ready(false),
    m_connectionState(Disconnected),
    m_error(NoError),
    m_errorString(""),
    m_model(NULL),
    m_interpreterState(InterpreterStateUnset),
    m_interpreterNote(""),
    m_componentCompleted(false),
    m_context(NULL),
    m_statusSocket(NULL),
    m_previewSocket(NULL),
    m_previewUpdated(false)
{
    m_previewStatus.fileName = "test.ngc";
    m_previewStatus.lineNumber = 0;
}

QPreviewClient::~QPreviewClient()
{
    disconnectSockets();
}

/** componentComplete is executed when the QML component is fully loaded */
void QPreviewClient::componentComplete()
{
    m_componentCompleted = true;

    if (m_ready == true)    // the component was set to ready before it was completed
    {
        start();
    }

    QQuickItem::componentComplete();
}

void QPreviewClient::setReady(bool arg)
{
    if (m_ready != arg) {
        m_ready = arg;
        emit readyChanged(arg);

        if (m_componentCompleted == false)
        {
            return;
        }

        if (m_ready)
        {
            start();
        }
        else
        {
            stop();
        }
    }
}

void QPreviewClient::start()
{
#ifdef QT_DEBUG
    DEBUG_TAG(1, "preview", "start")
#endif
    updateState(Connecting);

    if (connectSockets())
    {
        // do something
        m_statusSocket->subscribeTo("status");
        m_previewSocket->subscribeTo("preview");
    }
}

void QPreviewClient::stop()
{
#ifdef QT_DEBUG
    DEBUG_TAG(1, "preview", "stop")
#endif

    // cleanup here
    disconnectSockets();

    updateState(Disconnected);
    updateError(NoError, "");   // clear the error here
}

void QPreviewClient::updateState(QPreviewClient::State state)
{
    if (state != m_connectionState)
    {
        m_connectionState = state;
        emit connectionStateChanged(m_connectionState);
    }
}

void QPreviewClient::updateError(QPreviewClient::ConnectionError error, QString errorString)
{
    m_error = error;
    m_errorString = errorString;

    emit errorStringChanged(m_errorString);
    emit errorChanged(m_error);
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

        if ((m_interpreterState == InterpreterIdle)
                && m_previewUpdated
                && m_model)
        {
            m_model->endUpdate();
        }
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
        if (m_model == NULL)
        {
            return;
        }

        for (int i = 0; i < m_rx.preview_size(); ++i)
        {
            QList<pb::Preview> *previewList;
            pb::Preview preview;

            preview = m_rx.preview(i);

            if (preview.has_line_number())
            {
                m_previewStatus.lineNumber = preview.line_number();
            }

            if (preview.has_filename())
            {
                m_previewStatus.fileName = QString::fromStdString(preview.filename());
            }

            previewList = static_cast<QList<pb::Preview>*>(m_model->data(m_previewStatus.fileName,
                                                                         m_previewStatus.lineNumber,
                                                                         QGCodeProgramModel::PreviewRole).value<void*>());
            if (previewList == NULL)
            {
                previewList = new QList<pb::Preview>();
            }

            previewList->append(preview);

            m_model->setData(m_previewStatus.fileName, m_previewStatus.lineNumber,
                             QVariant::fromValue(static_cast<void*>(previewList)),
                             QGCodeProgramModel::PreviewRole);

            m_previewUpdated = true;
        }
    }
}

void QPreviewClient::pollError(int errorNum, const QString &errorMsg)
{
    QString errorString;
    errorString = QString("Error %1: ").arg(errorNum) + errorMsg;
    updateError(SocketError, errorString);
    updateState(Error);
}

/** Connects the 0MQ sockets */
bool QPreviewClient::connectSockets()
{
    m_context = new PollingZMQContext(this, 1);
    connect(m_context, SIGNAL(pollError(int,QString)),
            this, SLOT(pollError(int,QString)));
    m_context->start();

    m_statusSocket = m_context->createSocket(ZMQSocket::TYP_SUB, this);
    m_statusSocket->setLinger(0);

    m_previewSocket = m_context->createSocket(ZMQSocket::TYP_SUB, this);
    m_previewSocket->setLinger(0);

    try {
        m_statusSocket->connectTo(m_statusUri);
        m_previewSocket->connectTo(m_previewUri);
    }
    catch (zmq::error_t e) {
        QString errorString;
        errorString = QString("Error %1: ").arg(e.num()) + QString(e.what());
        updateError(SocketError, errorString);
        updateState(Error);
        return false;
    }

    connect(m_statusSocket, SIGNAL(messageReceived(QList<QByteArray>)),
            this, SLOT(statusMessageReceived(QList<QByteArray>)));
    connect(m_previewSocket, SIGNAL(messageReceived(QList<QByteArray>)),
            this, SLOT(previewMessageReceived(QList<QByteArray>)));

#ifdef QT_DEBUG
    DEBUG_TAG(1, "preview", "sockets connected" << m_statusUri << m_previewUri)
#endif

    return true;
}

/** Disconnects the 0MQ sockets */
void QPreviewClient::disconnectSockets()
{
    if (m_statusSocket != NULL)
    {
        m_statusSocket->close();
        m_statusSocket->deleteLater();
        m_statusSocket = NULL;
    }

    if (m_previewSocket != NULL)
    {
        m_previewSocket->close();
        m_previewSocket->deleteLater();
        m_previewSocket = NULL;
    }

    if (m_context != NULL)
    {
        m_context->stop();
        m_context->deleteLater();
        m_context = NULL;
    }
}
