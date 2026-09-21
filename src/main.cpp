#include <QCoreApplication>
#include "dds/dds.hpp"
#include "comms/rcwssubscriber.h"
#include "comms/telemetrypublisher.h"
#include <QDebug>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    qDebug() << "=== Memulai RCWS Gateway (Qt6 + CycloneDDS) ===";

    try {
        dds::domain::DomainParticipant participant{org::eclipse::cyclonedds::domain::default_id()};

        TelemetryPublisher telemetryPub(participant, true);
        RcwsSubscriber rcwsSubs(participant);

        return a.exec();
    } catch (const dds::core::Exception& e) {
        qCritical() << "Fatal DDS Error:" << e.what();
        return -1;
    }
}
