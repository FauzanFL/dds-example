#ifndef RCWSSUBSCRIBER_H
#define RCWSSUBSCRIBER_H

#include <QObject>
#include <dds/dds.h>
#include "RcwsCommand.h"
#include <thread>
#include <atomic>

class RcwsSubscriber : public QObject {
    Q_OBJECT
public:
    explicit RcwsSubscriber(dds_entity_t participant, QObject* parent = nullptr);
    ~RcwsSubscriber();

    void startListening();
    void stopListening();

signals:
    void commandReceived(float pan, float tilt, bool fire);

private:
    void listenerLoop();

    dds_entity_t participant_;
    dds_entity_t topic_;
    dds_entity_t reader_;

    std::thread thread_;
    std::atomic<bool> running_;
};

#endif // RCWSSUBSCRIBER_H
