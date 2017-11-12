#include <catch.hpp>
#include <trompeloeil.hpp>

#include <QtTest>
#include <QTemporaryDir>
#include <machinetalkservice.h>
#include <machinetalk/protobuf/status.pb.h>

using namespace machinetalk;
using namespace qtquickvcp;

TEST_CASE("MachinetalkService Tests", "[machinetalk]")
{
    QQmlEngine engine;
    QObject parent;

    const auto checkValue = [](QObject *output, const char *name, const QVariant::Type type) {
        const auto &value = output->property(name);
        REQUIRE(!value.isNull());
        REQUIRE(value.type() == type);
    };

    SECTION("creating motion object works") {
        QObject *output = MachinetalkService::recurseDescriptor(EmcStatusMotion::descriptor(), &parent);

        REQUIRE(output != nullptr);

        SECTION("converting int32 works") {
            checkValue(output, "activeQueue", QVariant::Int);
        }

        SECTION("converting bool works") {
            checkValue(output, "adaptiveFeedEnabled", QVariant::Bool);
        }

        SECTION("updating bool value works") {
            EmcStatusMotion message;
            message.set_adaptive_feed_enabled(true);

            MachinetalkService::recurseMessage(message, output);

            const auto &value = output->property("adaptiveFeedEnabled");
            REQUIRE(value.toBool() == true);
        }

        SECTION("converting double works") {
            checkValue(output, "currentVel", QVariant::Double);
        }

        SECTION("converting enum works") {
            checkValue(output, "state", QVariant::Int);
        }

        SECTION("converting nested message field works") {
            const auto &value = output->property("probedPosition");
            QObject *object = qvariant_cast<QObject *>(value);

            REQUIRE(object != nullptr);
            checkValue(object, "x", QVariant::Double);
        }

        SECTION("updating nested message field works") {
            EmcStatusMotion message;
            auto field = message.mutable_probed_position();
            field->set_x(1.3);

            MachinetalkService::recurseMessage(message, output);

            const auto &value = output->property("probedPosition");
            QObject *object = qvariant_cast<QObject *>(value);
            const auto &x = object->property("x");
            REQUIRE(x.toFloat() == Approx(1.3));
        }

        SECTION("position message also has index properties") {
            const auto &value = output->property("actualPosition");
            QObject *object = qvariant_cast<QObject *>(value);

            REQUIRE(object != nullptr);
            for (int i = 0; i < 9; ++i) {
                checkValue(object, QString::number(i).toLocal8Bit(), QVariant::Double);
            }
        }

        SECTION("updating position list of values works") {
            EmcStatusMotion message;
            auto field = message.mutable_actual_position();
            field->set_y(2.5);

            MachinetalkService::recurseMessage(message, output);

            const auto &value = output->property("actualPosition");
            QObject *object = qvariant_cast<QObject *>(value);
            REQUIRE(object != nullptr);
            REQUIRE(object->property("y").toDouble() == 2.5);
            REQUIRE(object->property("1").toDouble() == 2.5);
        }

        SECTION("repeated field with single property is represented as value") {
            const auto &value = qvariant_cast<QVariant>(output->property("din"));
            REQUIRE(value.toList().count() == 0);
        }

        SECTION("adding repeated field entry with single property works") {
            EmcStatusMotion message;
            auto field = message.add_din();
            field->set_index(0);
            field->set_value(true);

            MachinetalkService::recurseMessage(message, output);

            const auto &list = qvariant_cast<QVariant>(output->property("din")).toList();
            REQUIRE(list.count() == 1);
            REQUIRE(list.at(0) == true);

            SECTION("removing repeated field entry with single property works") {
                EmcStatusMotion message;
                auto field = message.add_din();
                field->set_index(0);

                MachinetalkService::recurseMessage(message, output);

                const auto &removedList = qvariant_cast<QVariant>(output->property("din")).toList();
                REQUIRE(removedList.count() == 0);
            }

        }

        SECTION("repeated field with multiple properties is represented as object") {
            const auto &value = qvariant_cast<QVariant>(output->property("axis"));
            REQUIRE(value.toList().count() == 0);
        }

        SECTION("adding repeated field entry with multiple properties works") {
            EmcStatusMotion message;
            auto field = message.add_axis();
            field->set_index(0);
            field->set_homing(false);
            field->set_homed(true);

            MachinetalkService::recurseMessage(message, output);

            const auto &list = qvariant_cast<QVariant>(output->property("axis")).toList();
            REQUIRE(list.count() == 1);
            QObject *object = qvariant_cast<QObject *>(list.at(0));
            REQUIRE(object != nullptr);
            REQUIRE(object->property("homing").toBool() == false);
            REQUIRE(object->property("homed").toBool() == true);

            SECTION("removing repeated field entry with multiple properties works") {
                EmcStatusMotion message;
                auto field = message.add_axis();
                field->set_index(0);

                MachinetalkService::recurseMessage(message, output);

                const auto &removedList = qvariant_cast<QVariant>(output->property("axis")).toList();
                REQUIRE(removedList.count() == 0);
            }

            SECTION("updating repeated field entry with multiple properties works") {
                EmcStatusMotion message;
                auto field = message.add_axis();
                field->set_index(0);
                field->set_homing(true);

                MachinetalkService::recurseMessage(message, output);

                const auto &updatedList = qvariant_cast<QVariant>(output->property("axis")).toList();
                REQUIRE(updatedList.count() == 1);
                REQUIRE(object->property("homing").toBool() == true);
            }
        }
    }

    SECTION("creating config object works") {
        QObject *output = MachinetalkService::recurseDescriptor(EmcStatusConfig::descriptor(), &parent);

        REQUIRE(output != nullptr);

        SECTION("converting string works") {
            const auto &value = output->property("grids");
            REQUIRE(value.type() == QVariant::String);
        }
    }

    SECTION("creating launcher object works") {
        QObject *output = MachinetalkService::recurseDescriptor(Launcher::descriptor(), &parent);

        REQUIRE(output != nullptr);

        SECTION("updating File object creates temporary file") {
            QTemporaryDir tempDir;
            tempDir.setAutoRemove(true);
            Launcher message;
            auto image = message.mutable_image();
            image->set_blob("very long and important data");
            image->set_name("test.txt");
            image->set_encoding(CLEARTEXT);

            MachinetalkService::recurseMessage(message, output, tempDir.path());

            QObject *object = qvariant_cast<QObject *>(output->property("image"));
            REQUIRE(object != nullptr);
            REQUIRE(object->property("url").isValid());
            const auto &url = object->property("url").toString();
            QFile file(QUrl(url).toLocalFile());
            REQUIRE(file.open(QIODevice::Text | QIODevice::ReadOnly));
            REQUIRE(file.readAll().toStdString() == "very long and important data");
            file.close();
        }
    }
}
