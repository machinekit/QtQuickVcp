#include "applicationlauncher.h"
#include "debughelper.h"

#if defined(Q_OS_IOS)
namespace gpb = google_public::protobuf;
#else
namespace gpb = google::protobuf;
#endif

using namespace nzmqt;

namespace qtquickvcp {

ApplicationLauncher::ApplicationLauncher(QObject *parent) :
    application::LauncherBase(parent),
    m_launchers(QJsonValue(QJsonArray())),
    m_synced(false)
{
    initializeObject();
    addLauncherTopic("launcher");
}

ApplicationLauncher::~ApplicationLauncher()
{
    MachinetalkService::removeTempPath("launcher"); // clean up dir created by json
}

void ApplicationLauncher::start(int index)
{
    if (!m_synced) {
        return;
    }

#ifdef QT_DEBUG
    DEBUG_TAG(1, debugName(), "starting launcher" << index)
#endif

    m_tx.set_index(index);
    sendLauncherStart(m_tx);
}

void ApplicationLauncher::kill(int index)
{
    if (!m_synced) {
        return;
    }

    m_tx.set_index(index);
    sendLauncherKill(m_tx);
}

void ApplicationLauncher::terminate(int index)
{
    if (!m_synced) {
        return;
    }

    m_tx.set_index(index);
    sendLauncherTerminate(m_tx);
}

void ApplicationLauncher::writeToStdin(int index, const QString &data)
{
    if (!m_synced) {
        return;
    }

    m_tx.set_index(index);
    m_tx.set_name(data.toStdString());
    sendLauncherWriteStdin(m_tx);
}

void ApplicationLauncher::call(const QString &command)
{
    if (!m_synced) {
        return;
    }

    m_tx.set_name(command.toStdString());
    sendLauncherCall(m_tx);
}

void ApplicationLauncher::shutdown()
{
    if (!m_synced) {
        return;
    }

    sendLauncherShutdown(m_tx);
}

void ApplicationLauncher::launcherFullUpdateReceived(const QByteArray &topic, const pb::Container &rx)
{
    Q_UNUSED(topic);
    m_launchers = QJsonValue(QJsonArray()); // clear old value
    MachinetalkService::updateValue(rx, &m_launchers, "launcher", "launcher"); // launcher protobuf value, launcher temp path
    emit launchersChanged(m_launchers);
}

void ApplicationLauncher::launcherIncrementalUpdateReceived(const QByteArray &topic, const pb::Container &rx)
{
    Q_UNUSED(topic);
    MachinetalkService::updateValue(rx, &m_launchers, "launcher", "launcher"); // launcher protobuf value, launcher temp path
    emit launchersChanged(m_launchers);
}

void ApplicationLauncher::syncStatus()
{
    m_synced = true;
    emit syncedChanged(m_synced);
}

void ApplicationLauncher::unsyncStatus()
{
    m_synced = false;
    emit syncedChanged(m_synced);
    initializeObject();
}

void ApplicationLauncher::initializeObject()
{
    m_launchers = QJsonValue(QJsonArray());
    emit launchersChanged(m_launchers);
}

}; // namespace qtquickvcp
