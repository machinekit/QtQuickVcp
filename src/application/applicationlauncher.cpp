#include "applicationlauncher.h"
#include <machinetalkservice.h>
#include "debughelper.h"

using namespace machinetalk;

namespace qtquickvcp {

ApplicationLauncher::ApplicationLauncher(QObject *parent) :
    application::LauncherBase(parent),
    m_launcherObject(nullptr),
    m_synced(false),
    m_temporaryDir(nullptr)
{
    initializeObject();
    createTemporaryDir();
    addLauncherTopic("launcher");
}

ApplicationLauncher::~ApplicationLauncher()
{
}

QVariant ApplicationLauncher::launchers() const
{
    if (m_launcherObject != nullptr) {
        return m_launcherObject->property("launcher");
    }
    else {
        return QVariant();
    }
}

bool ApplicationLauncher::isSynced() const
{
    return m_synced;
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

void ApplicationLauncher::setImportance(int index, int importance)
{
    if (!m_synced) {
        return;
    }

    auto launcher = m_tx.add_launcher();
    launcher->set_index(index);
    launcher->set_importance(static_cast<google::protobuf::uint32>(importance));

    sendLauncherSet(m_tx);
}

void ApplicationLauncher::handleLauncherFullUpdateMessage(const QByteArray &topic, const Container &rx)
{
    Q_UNUSED(topic);
    initializeObject(); // clear old value
    MachinetalkService::recurseMessage(rx, m_launcherObject, m_temporaryDir->path(), "launcher");
    emit launchersChanged();
}

void ApplicationLauncher::handleLauncherIncrementalUpdateMessage(const QByteArray &topic, const Container &rx)
{
    Q_UNUSED(topic);
    MachinetalkService::recurseMessage(rx, m_launcherObject, m_temporaryDir->path(), "launcher");
    emit launchersChanged();
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
    emit launchersChanged();
}

void ApplicationLauncher::initializeObject()
{
    if (m_launcherObject != nullptr) {
        m_launcherObject->deleteLater();
    }
    m_launcherObject = MachinetalkService::recurseDescriptor(machinetalk::Container::descriptor(), this, "launcher");
}

void ApplicationLauncher::createTemporaryDir()
{
    m_temporaryDir = std::make_unique<QTemporaryDir>();
    m_temporaryDir->setAutoRemove(true);
    if (!m_temporaryDir->isValid()) {
        qWarning() << "Cannot create temporary directory for application launcher";
    }
}

} // namespace qtquickvcp
