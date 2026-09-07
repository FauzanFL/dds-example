#include "telemetrypublisher.h"
#include <stdexcept>

TelemetryPublisher::TelemetryPublisher(dds_entity_t participant) {
    topic_ = dds_create_topic(participant, &messages_Telemetry_desc, "TelemetryTopic", NULL, NULL);
    if (topic_ < 0) throw std::runtime_error("Gagal memuat Telemetry Topic");

    writer_ = dds_create_writer(participant, topic_, NULL, NULL);
    if (writer_ < 0) throw std::runtime_error("Gagal membuat Telemetry Writer");
}

TelemetryPublisher::~TelemetryPublisher() {
    dds_delete(topic_);
    dds_delete(writer_);
}

void TelemetryPublisher::publish(double lat, double lon, float alt, float speed) {
    messages_Telemetry msg;
    msg.latitude = lat;
    msg.longitude = lon;
    msg.altitude = alt;
    msg.speed = speed;

    dds_write(writer_, &msg);
}