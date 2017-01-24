#include <QString>
#include <QtTest>
#include <halpin.h>

using namespace qtquickvcp;

class HalPinTest : public QObject
{
    Q_OBJECT

public:
    HalPinTest();

private Q_SLOTS:
    void testSyncing_data();
    void testDefaults();
    void testSyncing();
    void testSyncSequence();
    void testQueuing();
};

HalPinTest::HalPinTest()
{
}

void HalPinTest::testSyncing_data()
{
    QTest::addColumn<QVariant>("value");
    QTest::addColumn<bool>("synced");
    QTest::addColumn<bool>("result");

    QTest::newRow("0") << QVariant(true) << false << false;
    QTest::newRow("1") << QVariant(false) << false << false;
    QTest::newRow("2") << QVariant(true) << true << true;
    QTest::newRow("3") << QVariant(1.4) << false << false; // note we don't check the type yet
}

void HalPinTest::testDefaults()
{
    HalPin pin;
    QVERIFY2(pin.type() == HalPin::Bit, "defaut type wrong");
    QVERIFY2(pin.direction() == HalPin::Out, "default direction wrong");
    QVERIFY2(pin.synced() == false, "should not be synced per default");
    QVERIFY2(pin.enabled() == true, "should be enabled per default");
}

void HalPinTest::testSyncing()
{
    QFETCH(QVariant, value);
    QFETCH(bool, synced);
    QFETCH(bool, result);

    HalPin pin;
    pin.setType(HalPin::Bit);
    pin.setDirection(HalPin::Out);
    pin.setValue(value, synced);
    QVERIFY2(pin.synced() == result, "problem with sync");
}

void HalPinTest::testSyncSequence()
{
    HalPin pin;
    pin.setType(HalPin::Bit);
    pin.setDirection(HalPin::Out);

    pin.setValue(true, true);
    QVERIFY2(pin.synced() == true, "should be synced");

    pin.setValue(false, false);
    QVERIFY2(pin.synced() == false, "should not be synced");

    pin.setValue(true, false);
    QVERIFY2(pin.synced() == true, "should have stored last sync data");
}

void HalPinTest::testQueuing()
{
    HalPin pin;
    pin.setType(HalPin::Bit);
    pin.setDirection(HalPin::Out);
    pin.setQueuing(true);

    QSignalSpy syncedSpy(&pin, &HalPin::syncedChanged);
    QSignalSpy valueSpy(&pin, &HalPin::valueChanged);

    pin.setValue(true, true);
    QVERIFY2(pin.synced() == true, "should be synced");
    QVERIFY2(pin.value() == true, "value should be true");
    QVERIFY2(valueSpy.count() == 1, "sent one message");
    QVERIFY2(syncedSpy.count() == 1, "updated synced once");

    syncedSpy.clear();
    valueSpy.clear();
    pin.setValue(false, false);
    QVERIFY2(pin.synced() == false, "should not be synced");
    QVERIFY2(pin.value() == false, "value should be true");
    QVERIFY2(valueSpy.count() == 1, "sent one message");
    QVERIFY2(syncedSpy.count() == 1, "updated synced once");

    syncedSpy.clear();
    valueSpy.clear();
    pin.setValue(true, false);
    QVERIFY2(pin.synced() == false, "should not be synced");
    QVERIFY2(pin.value() == false, "value should be true");
    QVERIFY2(valueSpy.count() == 0, "sent no message");
    QVERIFY2(syncedSpy.count() == 0, "did not update synced");

    syncedSpy.clear();
    valueSpy.clear();
    pin.setValue(false, true);
    QVERIFY2(pin.synced() == false, "should not be synced");
    QVERIFY2(pin.value() == true, "value should be true");
    QVERIFY2(valueSpy.count() == 1, "sent one message");
    QVERIFY2(syncedSpy.count() == 2, "updated synced twice");

    syncedSpy.clear();
    valueSpy.clear();
    pin.setValue(true, true);
    QVERIFY2(pin.synced() == true, "should not be synced");
    QVERIFY2(pin.value() == true, "value should be true");
    QVERIFY2(valueSpy.count() == 0, "sent no messages");
    QVERIFY2(syncedSpy.count() == 1, "updated synced once");
}

QTEST_APPLESS_MAIN(HalPinTest)

#include "tst_halpintest.moc"
