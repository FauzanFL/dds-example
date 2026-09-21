#include "appcontroller.h"
#include <QDebug>
#include <stdexcept>

AppController::AppController(QObject *parent)
    : QObject{parent}, participant_(0)
{
    participant_ = dds_create_participant(DDS_DOMAIN_DEFAULT, NULL, NULL);
    if (participant_ < 0) {
        qCritical() << "Gagal inisialisasi DDS Participant";
        return;
    }

    telemetryPub_ = std::make_unique<TelemetryPublisher>(participant_);
    rcwsSub_ = std::make_unique<RcwsSubscriber>(participant_, this);

    connect(rcwsSub_.get(), &RcwsSubscriber::commandReceived,
            this, &AppController::onRcwsCommandReceived);

    connect(&timer_, &QTimer::timeout, this, &AppController::onTelemetryTimer);
}

AppController::~AppController()
{
    if (participant_ >= 0) {
        dds_delete(participant_);
    }
}

void AppController::start()
{
    if (rcwsSub_ && participant_ >= 0) {
        rcwsSub_->startListening();
        timer_.start(1000);
        dds_domainid_t active_domain_id;
        dds_get_domainid(participant_, &active_domain_id);
        qDebug() << "Aplikasi berbasis C API berjalan... Mempublikasikan Telemetry dan mendengarkan RCWS.";
        qDebug() << "Domain id: " << active_domain_id;
    }
}

void AppController::onTelemetryTimer() {
    currentLat += 0.0001;
    currentLon += 0.0001;
    telemetryPub_->publish(currentLat, currentLon, 150.0f, 60.5f);
    qDebug() << "Published Telemetry -> Lat:" << currentLat << "Lon:" << currentLon;
}

void AppController::onRcwsCommandReceived(float pan, float tilt, bool fire) {
    qDebug() << "=== RCWS COMMAND DITERIMA ===";
    qDebug() << "Pan:" << pan << "Tilt:" << tilt << "Fire:" << (fire ? "YES" : "NO");
}