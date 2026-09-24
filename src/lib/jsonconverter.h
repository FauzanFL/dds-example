#ifndef JSONCONVERTER_H
#define JSONCONVERTER_H

#include <QJsonObject>
#include <QString>

#include "RCWSStatus.hpp"
#include "RCWSCommand.hpp"
#include "Telemetry.hpp"
#include "MissionRoute.hpp"
#include "TargetTrack.hpp"

class JsonConverter
{
public:
    JsonConverter() = default;

    // =======================================================
    // 1. SERIALIZATION (DDS ke JSON) menggunakan OVERLOADING
    // =======================================================
    static QJsonObject toJson(const Weapon::RCWSStatus& data);
    static QJsonObject toJson(const Ship::Telemetry& data);
    static QJsonObject toJson(const Weapon::RCWSCommand& data);
    static QJsonObject toJson(const Ship::MissionRoute& data);
    static QJsonObject toJson(const Tactical::TargetTrack& data);

    // =======================================================
    // 2. DESERIALIZATION (JSON ke DDS) menggunakan TEMPLATE
    // =======================================================
    template <typename T>
    static T fromJson(const QJsonObject& json);

};

// --- DEKLARASI TEMPLATE SPECIALIZATION ---
// (Wajib diletakkan di luar deklarasi class pada file header)

template <>
Weapon::RCWSCommand JsonConverter::fromJson<Weapon::RCWSCommand>(const QJsonObject& json);

template <>
Weapon::RCWSStatus JsonConverter::fromJson<Weapon::RCWSStatus>(const QJsonObject& json);

template <>
Ship::Telemetry JsonConverter::fromJson<Ship::Telemetry>(const QJsonObject& json);

template <>
Ship::MissionRoute JsonConverter::fromJson<Ship::MissionRoute>(const QJsonObject& json);

template <>
Tactical::TargetTrack JsonConverter::fromJson<Tactical::TargetTrack>(const QJsonObject& json);

#endif // JSONCONVERTER_H
