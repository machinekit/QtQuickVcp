#include "applicationhelpers.h"
#include <QDesktopServices>

namespace qtquickvcp {

ApplicationHelpers::ApplicationHelpers(QObject *parent)
    : QObject(parent)
{

}

bool ApplicationHelpers::openUrlWithDefaultApplication(const QUrl &url) const
{
    return QDesktopServices::openUrl(url);
}

} // namespace qtquickvcp
