#include "applicationhelpers.h"
#include <QDesktopServices>

namespace qtquickvcp {

ApplicationHelpers::ApplicationHelpers(QQmlEngine *engine, QObject *parent)
    : QObject(parent)
    , m_engine(engine)
{

}

bool ApplicationHelpers::openUrlWithDefaultApplication(const QUrl &url) const
{
    return QDesktopServices::openUrl(url);
}

void ApplicationHelpers::clearQmlComponentCache() const
{
    m_engine->clearComponentCache();
}

} // namespace qtquickvcp
