#ifndef TELEMETRYPUBLISHER_H
#define TELEMETRYPUBLISHER_H

#include <QObject>
#include <QTimer>
#include <QString>
#include <dds/dds.hpp>
#include "Telemetry.hpp"
#include "TargetTrack.hpp"

class TelemetryPublisher : public QObject
{
    Q_OBJECT
public:
    explicit TelemetryPublisher(dds::domain::DomainParticipant participant, bool includeTarget = false, QObject* parent = nullptr);

public slots:
    void publishTelemetryData();
    void publishTargetData();

private:
    dds::pub::Publisher publisher_;
    dds::topic::Topic<Ship::Telemetry> telemetry_topic_;
    dds::topic::Topic<Tactical::TargetTrack> target_topic_;

    dds::pub::DataWriter<Ship::Telemetry> telemetry_writer_;
    dds::pub::DataWriter<Tactical::TargetTrack> target_writer_;
    QTimer* telemetry_timer_ = nullptr;
    QTimer* target_timer_ = nullptr;
    bool includeTarget_ = false;

    double ship_hdg_ = 45;
    double ship_lat_ = -7.350;
    double ship_lon_ = 113.050;
    double ship_speed_ = 21;
    std::string ship_id_;
};

#endif // TELEMETRYPUBLISHER_H
