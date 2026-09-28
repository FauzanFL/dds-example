#include <QTest>
#include <dds/dds.hpp>
#include "comms/telemetrypublisher.h"
#include "config/qos_profiles.h"
#include "Telemetry.hpp"

class TestDDS : public QObject
{
    Q_OBJECT

private slots:
    void testPublisherTelemetry()
    {
        dds::domain::DomainParticipant participant(99);
        TelemetryPublisher myPublisher(participant, false, nullptr);
        dds::topic::Topic<Ship::Telemetry> topic(participant, "Ship/Telemetry");

        dds::sub::qos::DataReaderQos readerQos = QoSProfile::BestEffortVolatile(dds::sub::Subscriber(participant));
        dds::sub::DataReader<Ship::Telemetry> dummyReader(dds::sub::Subscriber(participant), topic, readerQos);

        QTest::qWait(1000);
        Ship::Telemetry mockData;
        mockData.ship_id() = "KRI-TEST-01";
        mockData.speed() = 25.5;
        mockData.heading() = 90.0;

        myPublisher.publishTelemetryData();

        QTest::qWait(500);
        auto samples = dummyReader.take();

        QVERIFY2(samples.length() > 0, "Publisher tidak mengirim data apa pun ke DDS!");

        bool foundValid = false;
        for (const auto& sample: samples) {
            if (sample.info().valid()) {
                foundValid = true;
                break;
            }
        }

        QVERIFY2(foundValid, "Data yang dikirim oleh Publisher tidak valid!");
    }
};

QTEST_MAIN(TestDDS)
#include "TestDDS.moc"