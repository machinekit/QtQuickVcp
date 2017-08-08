#include <QString>
#include <QtTest>
#include <servicediscovery.h>

using namespace qtquickvcp;

class ServiceDiscoveryTest : public QObject
{
    Q_OBJECT

public:
    ServiceDiscoveryTest();

private Q_SLOTS:
    void setRunningSetsRunningStateToTrue();
};

ServiceDiscoveryTest::ServiceDiscoveryTest()
{
}

void ServiceDiscoveryTest::setRunningSetsRunningStateToTrue()
{
    ServiceDiscovery discovery;

    discovery.setRunning(true);

    QVERIFY2(discovery.isRunning() == true, "set running did not work");
}

QTEST_APPLESS_MAIN(ServiceDiscoveryTest)

#include "tst_servicediscoverytest.moc"
