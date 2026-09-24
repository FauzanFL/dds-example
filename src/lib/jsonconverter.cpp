#include "jsonconverter.h"

// =======================================================
// IMPLEMENTASI SERIALIZATION (OVERLOADING)
// =======================================================

QJsonObject JsonConverter::toJson(const Weapon::RCWSStatus &data)
{
    QJsonObject json;
    json["ship_id"] = QString::fromStdString(data.ship_id());
    json["rcws_id"] = QString::fromStdString(data.rcws_id());
    json["state"] = QString::fromStdString(data.state());
    json["ammo"] = data.ammo();
    return json;
}

QJsonObject JsonConverter::toJson(const Weapon::RCWSCommand &data)
{
    QJsonObject json;
    json["ship_id"] = QString::fromStdString(data.ship_id());
    json["rcws_id"] = QString::fromStdString(data.rcws_id());
    json["command_id"] = QString::fromStdString(data.command_id());
    json["azimuth"] = data.azimuth();
    json["elevation"] = data.elevation();
    json["range"] = data.range();
    return json;
}

QJsonObject JsonConverter::toJson(const Ship::MissionRoute &data)
{
    QJsonObject json;
    json["ship_id"] = QString::fromStdString(data.ship_id());
    json["waypoint"] = QString::fromStdString(data.waypoint());
    json["latitude"] = data.latitude();
    json["longitude"] = data.longitude();
    return json;
}

QJsonObject JsonConverter::toJson(const Ship::Telemetry &data)
{
    QJsonObject json;
    json["ship_id"] = QString::fromStdString(data.ship_id());
    json["speed"] = data.speed();
    json["heading"] = data.heading();
    json["latitude"] = data.latitude();
    json["longitude"] = data.longitude();
    return json;
}

QJsonObject JsonConverter::toJson(const Tactical::TargetTrack &data)
{
    QJsonObject json;
    json["distance"] = data.distance();
    json["bearing"] = data.bearing();
    json["latitude"] = data.latitude();
    json["longitude"] = data.longitude();
    return json;
}

// =======================================================
// IMPLEMENTASI DESERIALIZATION (TEMPLATE SPECIALIZATION)
// =======================================================

template <>
Weapon::RCWSStatus JsonConverter::fromJson<Weapon::RCWSStatus>(const QJsonObject& json)
{
    Weapon::RCWSStatus status;
    status.ship_id() = json["ship_id"].toString("UNKNOWN").toStdString();
    status.rcws_id() = json["rcws_id"].toString("UNKNOWN").toStdString();
    status.state() = json["state"].toString("UNKNOWN").toStdString();
    status.ammo() = json["ammo"].toInteger(0);
    return status;
}

template <>
Weapon::RCWSCommand JsonConverter::fromJson<Weapon::RCWSCommand>(const QJsonObject& json)
{
    Weapon::RCWSCommand cmd;
    cmd.ship_id() = json["ship_id"].toString("UNKNOWN").toStdString();
    cmd.rcws_id() = json["rcws_id"].toString("UNKNOWN").toStdString();
    cmd.command_id() = json["command_id"].toString("UNKNOWN").toStdString();
    cmd.azimuth() = json["azimuth"].toDouble(0.0);
    cmd.elevation() = json["elevation"].toDouble(0.0);
    cmd.range() = json["range"].toDouble(0.0);
    return cmd;
}

template <>
Ship::MissionRoute JsonConverter::fromJson<Ship::MissionRoute>(const QJsonObject& json)
{
    Ship::MissionRoute route;
    route.ship_id() = json["ship_id"].toString("UNKNOWN").toStdString();
    route.waypoint() = json["waypoint"].toString("UNKNOWN").toStdString();
    route.latitude() = json["latitude"].toDouble(0.0);
    route.longitude() = json["longitude"].toDouble(0.0);
    return route;
}

template <>
Ship::Telemetry JsonConverter::fromJson<Ship::Telemetry>(const QJsonObject& json)
{
    Ship::Telemetry telemetry;
    telemetry.ship_id() = json["ship_id"].toString("UNKNOWN").toStdString();
    telemetry.speed() = json["speed"].toDouble(0.0);
    telemetry.heading() = json["heading"].toDouble(0.0);
    telemetry.latitude() = json["latitude"].toDouble(0.0);
    telemetry.longitude() = json["longitude"].toDouble(0.0);
    return telemetry;
}

template <>
Tactical::TargetTrack JsonConverter::fromJson<Tactical::TargetTrack>(const QJsonObject& json)
{
    Tactical::TargetTrack target;
    target.distance() = json["distance"].toDouble(0.0);
    target.bearing() = json["bearing"].toDouble(0.0);
    target.latitude() = json["latitude"].toDouble(0.0);
    target.longitude() = json["longitude"].toDouble(0.0);
    return target;
}