#include <QObject>
#include <QtTest/QTest>
#include "../src/lib/jsonconverter.h"

class TestSerialization : public QObject
{
    Q_OBJECT
private slots:

    // ---------------------------------------------------------
    // TEST 1: Menguji Serialization (DDS ke JSON)
    // ---------------------------------------------------------
    void testStatusSerialization()
    {
        Ship::Telemetry data;
        data.ship_id() = "KRI-123";
        data.heading() = 21.0;
        data.speed() = 15.0;
        data.latitude() = -7.792661;
        data.longitude() = 110.365922;

        QJsonObject json = JsonConverter::toJson(data);
        QCOMPARE(json["ship_id"].toString(), QString("KRI-123"));
        QCOMPARE(json["speed"].toDouble(), 15.0);
        QCOMPARE(json["heading"].toDouble(), 21.0);
        QCOMPARE(json["latitude"].toDouble(), -7.792661);
        QCOMPARE(json["longitude"].toDouble(), 110.365922);
    }

    // ---------------------------------------------------------
    // TEST 2: Menguji Deserialization (JSON ke DDS) Normal
    // ---------------------------------------------------------
    void testCommandDeserialization()
    {
        QJsonObject json;
        json["ship_id"] = "KRI-444";
        json["rcws_id"] = "TURRET";
        json["state"] = "IDLE";
        json["ammo"] = 21;

        Weapon::RCWSStatus status = JsonConverter::fromJson<Weapon::RCWSStatus>(json);

        QCOMPARE(QString::fromStdString(status.ship_id()), QString("KRI-444"));
        QCOMPARE(QString::fromStdString(status.rcws_id()), QString("TURRET"));
        QCOMPARE(QString::fromStdString(status.state()), QString("IDLE"));
        QCOMPARE(status.ammo(), 21);
    }

    // ---------------------------------------------------------
    // TEST 3: Edge Case - JSON Tidak Lengkap (Menjamin 100% Coverage)
    // ---------------------------------------------------------
    void testCommandDeserialization_MissingKeys()
    {
        QJsonObject json;

        Weapon::RCWSCommand cmd = JsonConverter::fromJson<Weapon::RCWSCommand>(json);
        QCOMPARE(QString::fromStdString(cmd.ship_id()), QString("UNKNOWN"));
    }
};

QTEST_APPLESS_MAIN(TestSerialization)
#include "TestSerialization.moc"