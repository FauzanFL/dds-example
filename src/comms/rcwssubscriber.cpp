#include "rcwssubscriber.h"
#include <QDebug>
#include "../config/qos_profiles.h"

RcwsSubscriber::RcwsSubscriber(dds::domain::DomainParticipant participant, QObject* parent)
    : QObject(parent),
    subscriber_(participant),
    mission_listener_(this), // Lempar 'this' agar Listener bisa memicu Signal Qt
    command_listener_(this),
    status_listener_(this),
    mission_topic_(participant, "Ship/MissionRoute"),
    command_topic_(participant, "Weapon/RCWSCommand"),
    status_topic_(participant, "Weapon/RCWSStatus"),
    mission_reader_(subscriber_, mission_topic_, QoSProfile::CommandSub(subscriber_), &mission_listener_, dds::core::status::StatusMask::data_available()),
    command_reader_(subscriber_, command_topic_, QoSProfile::CommandSub(subscriber_), &command_listener_, dds::core::status::StatusMask::data_available()),
    status_reader_(subscriber_, status_topic_, QoSProfile::StatusSub(subscriber_), &status_listener_, dds::core::status::StatusMask::data_available())
{
    qDebug() << "[Subscriber] Siap mendengarkan Mission, Command, dan Status...";
}

void MissionListener::on_data_available(dds::sub::DataReader<Ship::MissionRoute>& reader) {
    auto samples = reader.take(); // Ambil data dari antrean DDS
    for (const auto& sample : samples) {
        if (sample.info().valid()) {
            // Lempar data ke thread utama melalui Signal Qt
            const auto& data = sample.data();
            qDebug().noquote() << "[Subscriber] Received MissionRoute Data";
            qDebug() << "SHIP-ID: " << QString::fromStdString(data.ship_id())
                     << " | Waypoint: " << QString::fromStdString(data.waypoint())
                     << " | Latitude: " << data.latitude()
                     << " | Longitude: " << data.longitude();
            emit parent_->missionReceived(sample.data());
        }
    }
}

void CommandListener::on_data_available(dds::sub::DataReader<Weapon::RCWSCommand>& reader) {
    auto samples = reader.take();
    for (const auto& sample : samples) {
        if (sample.info().valid()) {
            const auto& data = sample.data();
            qDebug().noquote() << "[Subscriber] Received Command Data";
            qDebug() << "SHIP-ID: " << QString::fromStdString(data.ship_id())
                     << " | RCWS-ID: " << QString::fromStdString(data.rcws_id())
                     << " | CommandId: " << QString::fromStdString(data.command_id())
                     << " | Azimuth: " << data.azimuth()
                     << " | Elevation: " << data.elevation()
                     << " | Range: " << data.range();
            emit parent_->commandReceived(sample.data());
        }
    }
}

void StatusListener::on_data_available(dds::sub::DataReader<Weapon::RCWSStatus>& reader) {
    auto samples = reader.take();
    for (const auto& sample : samples) {
        if (sample.info().valid()) {
            const auto& data = sample.data();
            qDebug().noquote() << "[Subscriber] Received Status Data: ";
            qDebug() << "SHIP-ID: " << QString::fromStdString(data.ship_id())
                     << " | RCWS-ID: " << QString::fromStdString(data.rcws_id())
                     << " | State: " << QString::fromStdString(data.state())
                     << " | Ammo: " << data.ammo();
            emit parent_->statusReceived(sample.data());
        }
    }
}