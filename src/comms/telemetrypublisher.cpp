#include "telemetrypublisher.h"
#include <QDebug>
#include "../config/qos_profiles.h"

TelemetryPublisher::TelemetryPublisher(dds::domain::DomainParticipant participant, bool includeTarget, QObject* parent)
    : QObject(parent),
    publisher_(participant),
    telemetry_topic_(participant, "Ship/Telemetry"),
    target_topic_(participant, "Tactical/TargetTrack"),
    telemetry_writer_(publisher_, telemetry_topic_, QoSProfile::TelemetryPub(publisher_)),
    target_writer_(publisher_, target_topic_, QoSProfile::TelemetryPub(publisher_)),
    includeTarget_(includeTarget)
{
    telemetry_timer_ = new QTimer(this);
    connect(telemetry_timer_, &QTimer::timeout, this, &TelemetryPublisher::publishTelemetryData);
    telemetry_timer_->start(3000);
    qDebug() << "[Publisher] Telemetry Broadcaster started.";

    if (includeTarget_) {
        target_timer_ = new QTimer(this);
        connect(target_timer_, &QTimer::timeout, this, &TelemetryPublisher::publishTargetData);
        target_timer_->start(10000);
        qDebug() << "[Publisher] Target Broadcaster started.";
    }
}

void TelemetryPublisher::publishTelemetryData() {
    ship_hdg_ += 0.1;
    ship_lat_ += 0.0001;
    ship_lon_ += 0.0001;
    ship_speed_ += 0.1;

    Ship::Telemetry telemetryData;
    telemetryData.ship_id() = "KRI-247";
    telemetryData.heading() = ship_hdg_;
    telemetryData.latitude() = ship_lat_;
    telemetryData.longitude() = ship_lon_;
    telemetryData.speed() = ship_speed_;

    telemetry_writer_.write(telemetryData);

    qDebug() << "[Publisher] Mengirim Telemetry Data";
}

void TelemetryPublisher::publishTargetData() {
    Tactical::TargetTrack target;
    target.bearing() = 12.1;
    target.distance() = 5.1;
    target.latitude() = -8;
    target.longitude() = 114;

    target_writer_.write(target);
    qDebug() << "[Publisher] Mengirim Target Data";
}