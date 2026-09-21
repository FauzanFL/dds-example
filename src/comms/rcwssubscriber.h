#ifndef RCWSSUBSCRIBER_H
#define RCWSSUBSCRIBER_H

#include <QObject>
#include <dds/dds.hpp>
#include "RCWSCommand.hpp"
#include "RCWSStatus.hpp"
#include "MissionRoute.hpp"

class RcwsSubscriber;

class MissionListener : public dds::sub::NoOpDataReaderListener<Ship::MissionRoute> {
public:
    explicit MissionListener(RcwsSubscriber* parent) : parent_(parent) {}
    void on_data_available(dds::sub::DataReader<Ship::MissionRoute>& reader) override;
private:
    RcwsSubscriber* parent_;
};

class CommandListener : public dds::sub::NoOpDataReaderListener<Weapon::RCWSCommand> {
public:
    explicit CommandListener(RcwsSubscriber* parent) : parent_(parent) {}
    void on_data_available(dds::sub::DataReader<Weapon::RCWSCommand>& reader) override;
private:
    RcwsSubscriber* parent_;
};

class StatusListener : public dds::sub::NoOpDataReaderListener<Weapon::RCWSStatus> {
public:
    explicit StatusListener(RcwsSubscriber* parent) : parent_(parent) {}
    void on_data_available(dds::sub::DataReader<Weapon::RCWSStatus>& reader) override;
private:
    RcwsSubscriber* parent_;
};

class RcwsSubscriber : public QObject {
    Q_OBJECT
public:
    explicit RcwsSubscriber(dds::domain::DomainParticipant participant, QObject* parent = nullptr);

    void startListening();
    void stopListening();

signals:
    // Sinyal Qt yang akan dilempar ke sistem UI atau Logika Utama
    void missionReceived(const Ship::MissionRoute& data);
    void commandReceived(const Weapon::RCWSCommand& data);
    void statusReceived(const Weapon::RCWSStatus& data);

private:
    dds::sub::Subscriber subscriber_;

    MissionListener mission_listener_;
    CommandListener command_listener_;
    StatusListener status_listener_;

    dds::topic::Topic<Ship::MissionRoute> mission_topic_;
    dds::topic::Topic<Weapon::RCWSCommand> command_topic_;
    dds::topic::Topic<Weapon::RCWSStatus> status_topic_;

    dds::sub::DataReader<Ship::MissionRoute> mission_reader_;
    dds::sub::DataReader<Weapon::RCWSCommand> command_reader_;
    dds::sub::DataReader<Weapon::RCWSStatus> status_reader_;
};

#endif // RCWSSUBSCRIBER_H
