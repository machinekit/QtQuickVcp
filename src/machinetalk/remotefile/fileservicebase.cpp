/****************************************************************************
**
** This file was generated by a code generator based on imatix/gsl
** Any changes in this file will be lost.
**
****************************************************************************/
#include "fileservicebase.h"
#include <google/protobuf/text_format.h>
#include "debughelper.h"

#if defined(Q_OS_IOS)
namespace gpb = google_public::protobuf;
#else
namespace gpb = google::protobuf;
#endif

using namespace nzmqt;

namespace machinetalk { namespace remotefile {

/** Generic File Service Base implementation */
FileServiceBase::FileServiceBase(QObject *parent)
    : QObject(parent)
    , QQmlParserStatus()
    , m_componentCompleted(false)
    , m_ready(false)
    , m_debugName(QStringLiteral("File Service Base"))
    , m_file2Channel(nullptr)
    , m_state(State::Down)
    , m_previousState(State::Down)
    , m_errorString(QStringLiteral(""))
{
    // initialize file2 channel
    m_file2Channel = new common::RpcService(this);
    m_file2Channel->setDebugName(m_debugName + " - file2");
    connect(m_file2Channel, &common::RpcService::socketUriChanged,
            this, &FileServiceBase::file2UriChanged);
    connect(m_file2Channel, &common::RpcService::stateChanged,
            this, &FileServiceBase::file2ChannelStateChanged);
    connect(m_file2Channel, &common::RpcService::socketMessageReceived,
            this, &FileServiceBase::processFile2ChannelMessage);
    // state machine
    connect(this, &FileServiceBase::fsmDownStart,
            this, &FileServiceBase::fsmDownStartEvent);
    connect(this, &FileServiceBase::fsmTryingFile2Up,
            this, &FileServiceBase::fsmTryingFile2UpEvent);
    connect(this, &FileServiceBase::fsmUpStop,
            this, &FileServiceBase::fsmUpStopEvent);
}

FileServiceBase::~FileServiceBase()
{
}

void FileServiceBase::startFile2Channel()
{
    m_file2Channel->setReady(true);
}

void FileServiceBase::stopFile2Channel()
{
    m_file2Channel->setReady(false);
}

/** Processes all message received on file2 */
void FileServiceBase::processFile2ChannelMessage(const QByteArray &topic, const Container &rx)
{

    // react to ping message
    if (rx.type() == MT_PING)
    {
        handlePingMessage(topic, rx);
    }

    // react to file get message
    else if (rx.type() == MT_FILE_GET)
    {
        handleFileGetMessage(topic, rx);
    }

    // react to file put message
    else if (rx.type() == MT_FILE_PUT)
    {
        handleFilePutMessage(topic, rx);
    }

    // react to file ls message
    else if (rx.type() == MT_FILE_LS)
    {
        handleFileLsMessage(topic, rx);
    }

    // react to file mkdir message
    else if (rx.type() == MT_FILE_MKDIR)
    {
        handleFileMkdirMessage(topic, rx);
    }

    // react to file delete message
    else if (rx.type() == MT_FILE_DELETE)
    {
        handleFileDeleteMessage(topic, rx);
    }

    emit file2MessageReceived(topic, rx);
}

void FileServiceBase::sendFile2Message(const QByteArray &topic, ContainerType type, Container &tx)
{
    m_file2Channel->sendSocketMessage(topic, type, tx);
}

void FileServiceBase::sendPingAcknowledge(const QByteArray &topic, Container &tx)
{
    sendFile2Message(topic, MT_PING_ACKNOWLEDGE, tx);
}

void FileServiceBase::sendFileData(const QByteArray &topic, Container &tx)
{
    sendFile2Message(topic, MT_FILE_DATA, tx);
}

void FileServiceBase::sendFileListing(const QByteArray &topic, Container &tx)
{
    sendFile2Message(topic, MT_FILE_LISTING, tx);
}

void FileServiceBase::sendCmdComplete(const QByteArray &topic, Container &tx)
{
    sendFile2Message(topic, MT_CMD_COMPLETE, tx);
}

void FileServiceBase::sendError(const QByteArray &topic, Container &tx)
{
    sendFile2Message(topic, MT_ERROR, tx);
}

void FileServiceBase::fsmDown()
{
#ifdef QT_DEBUG
    DEBUG_TAG(1, m_debugName, "State DOWN");
#endif
    m_state = State::Down;
    emit stateChanged(m_state);
}

void FileServiceBase::fsmDownStartEvent()
{
    if (m_state == State::Down)
    {
#ifdef QT_DEBUG
        DEBUG_TAG(1, m_debugName, "Event START");
#endif
        // handle state change
        emit fsmDownExited(QPrivateSignal());
        fsmTrying();
        emit fsmTryingEntered(QPrivateSignal());
        // execute actions
        startFile2Channel();
     }
}

void FileServiceBase::fsmTrying()
{
#ifdef QT_DEBUG
    DEBUG_TAG(1, m_debugName, "State TRYING");
#endif
    m_state = State::Trying;
    emit stateChanged(m_state);
}

void FileServiceBase::fsmTryingFile2UpEvent()
{
    if (m_state == State::Trying)
    {
#ifdef QT_DEBUG
        DEBUG_TAG(1, m_debugName, "Event FILE2 UP");
#endif
        // handle state change
        emit fsmTryingExited(QPrivateSignal());
        fsmUp();
        emit fsmUpEntered(QPrivateSignal());
        // execute actions
     }
}

void FileServiceBase::fsmUp()
{
#ifdef QT_DEBUG
    DEBUG_TAG(1, m_debugName, "State UP");
#endif
    m_state = State::Up;
    emit stateChanged(m_state);
}

void FileServiceBase::fsmUpStopEvent()
{
    if (m_state == State::Up)
    {
#ifdef QT_DEBUG
        DEBUG_TAG(1, m_debugName, "Event STOP");
#endif
        // handle state change
        emit fsmUpExited(QPrivateSignal());
        fsmDown();
        emit fsmDownEntered(QPrivateSignal());
        // execute actions
        stopFile2Channel();
     }
}

void FileServiceBase::file2ChannelStateChanged(common::RpcService::State state)
{

    if (state == common::RpcService::State::Up)
    {
        if (m_state == State::Trying)
        {
            emit fsmTryingFile2Up(QPrivateSignal());
        }
    }
}

/** start trigger function */
void FileServiceBase::start()
{
    if (m_state == State::Down) {
        emit fsmDownStart(QPrivateSignal());
    }
}

/** stop trigger function */
void FileServiceBase::stop()
{
    if (m_state == State::Up) {
        emit fsmUpStop(QPrivateSignal());
    }
}

} } // namespace machinetalk::remotefile
