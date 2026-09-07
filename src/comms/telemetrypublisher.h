#ifndef TELEMETRYPUBLISHER_H
#define TELEMETRYPUBLISHER_H

#include <dds/dds.h>
#include <Telemetry.h>

class TelemetryPublisher
{
public:
    explicit TelemetryPublisher(dds_entity_t participant);
    ~TelemetryPublisher();

    void publish(double lat, double lon, float alt, float speed);

private:
    dds_entity_t topic_;
    dds_entity_t writer_;
};

#endif // TELEMETRYPUBLISHER_H
