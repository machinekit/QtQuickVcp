#include <catch.hpp>
#include <trompeloeil.hpp>

#include <QString>
#include <QtTest>
#include <halpin.h>

using namespace qtquickvcp;

TEST_CASE("HAL pin tests", "[halremote]")
{
    SECTION("syncing works") {
        GIVEN("we have HAL pin of type bit and with direction out") {
            HalPin pin;
            pin.setType(HalPin::Bit);
            pin.setDirection(HalPin::Out);

            WHEN("we set the pin to true without sync") {
                pin.setValue(QVariant(true), false);

                THEN("the pin should still by out of sync") {
                    REQUIRE(pin.synced() == false);
                }
            }

            WHEN("we set the pin to false without sync") {
                pin.setValue(QVariant(false), false);

                THEN("the pin should still be out of sync") {
                    REQUIRE(pin.synced() == false);
                }
            }

            WHEN("we set the pin to true with sync") {
                pin.setValue(QVariant(true), true);

                THEN("the pin should be synced") {
                    REQUIRE(pin.synced() == true);
                }
            }

            WHEN("we set the pin to 1.4 without sync") {
                pin.setValue(QVariant(1.4), false);

                THEN("the pin should still be out of sync") {
                    REQUIRE(pin.synced() == false);
                }
            }

            WHEN("we set the pin to true with sync") {
                pin.setValue(true, true);

                THEN("the pin should be in sync") {
                    REQUIRE(pin.synced() == true);
                }

                AND_WHEN("we set the pin to false without sync") {
                    pin.setValue(false, false);

                    THEN("the pin should not be synced") {
                        REQUIRE(pin.synced() == false);
                    }

                    AND_WHEN("we set the pin to true again without sync") {
                        pin.setValue(true, false);

                        THEN("the pin should restore the last sync data") {
                            REQUIRE(pin.synced() == true);
                        }
                    }
                }
            }
        }
    }

    SECTION("defaults are correct") {
        HalPin pin;
        REQUIRE(pin.type() == HalPin::Bit);
        REQUIRE(pin.direction() == HalPin::Out);
        REQUIRE(pin.synced() == false);
        REQUIRE(pin.enabled() == true);
    }
}
