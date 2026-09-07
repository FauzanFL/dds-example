#ifndef APPCONTROLLER_H
#define APPCONTROLLER_H

#include <QObject>
#include <QTimer>
#include <memory>
#include <dds/dds.h>
#include "comms/rcwssubscriber.h"
#include "comms/telemetrypublisher.h"

class AppController : public QObject
{
    Q_OBJECT
public:
    explicit AppController(QObject *parent = nullptr);
    ~AppController();
    void start();

private slots:
    void onTelemetryTimer();
    void onRcwsCommandReceived(float pan, float tilt, bool fire);

private:
    dds_entity_t participant_;

    std::unique_ptr<TelemetryPublisher> telemetryPub_;
    std::unique_ptr<RcwsSubscriber> rcwsSub_;
    QTimer timer_;

    double currentLat = -7.7956;
    double currentLon = 110.3695;
};

#endif // APPCONTROLLER_H
