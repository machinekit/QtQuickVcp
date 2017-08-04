#include <QString>
#include <QtTest>

class ServiceDiscoveryTest : public QObject
{
    Q_OBJECT

public:
    ServiceDiscoveryTest();

private Q_SLOTS:
    void testCase1();
};

ServiceDiscoveryTest::ServiceDiscoveryTest()
{
}

void ServiceDiscoveryTest::testCase1()
{
    QVERIFY2(true, "Failure");
}

QTEST_APPLESS_MAIN(ServiceDiscoveryTest)

#include "tst_servicediscoverytest.moc"
