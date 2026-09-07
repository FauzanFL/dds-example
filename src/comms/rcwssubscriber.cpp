#include "rcwssubscriber.h"
#include <stdexcept>
#include <QDebug>

RcwsSubscriber::RcwsSubscriber(dds_entity_t participant, QObject* parent)
    : QObject(parent), participant_(participant), running_(false)
{
    topic_ = dds_create_topic(participant, &messages_RcwsCommand_desc, "RcwsCommandTopic", NULL, NULL);
    if (topic_ < 0) throw std::runtime_error("Gagal membuat RcwsCommand Topic");

    reader_ = dds_create_reader(participant, topic_, NULL, NULL);
    if (reader_ < 0) throw std::runtime_error("Gagal membuat RcwsCommand Reader");
}

RcwsSubscriber::~RcwsSubscriber()
{
    stopListening();
    dds_delete(reader_);
    dds_delete(topic_);
}

void RcwsSubscriber::startListening()
{
    if (!running_) {
        running_ = true;
        thread_ = std::thread(&RcwsSubscriber::listenerLoop, this);
    }
}

void RcwsSubscriber::stopListening()
{
    running_ = false;
    if (thread_.joinable()) {
        thread_.join();
    }
}

void RcwsSubscriber::listenerLoop()
{
    dds_entity_t waitset = dds_create_waitset(participant_);
    if (waitset < 0) {
        qCritical() << "Gagal membuat WaitSet pada Subscriber!";
        return;
    }
    dds_entity_t readcond = dds_create_readcondition(reader_, DDS_DATA_AVAILABLE_STATUS);
    if (readcond < 0) {
        qCritical() << "Gagal membuat ReadCondition!";
        dds_delete(waitset);
        return;
    }

    dds_waitset_attach(waitset, readcond, readcond);

    while (running_) {
        dds_attach_t wsresults[1];
        dds_return_t num_events = dds_waitset_wait(waitset, wsresults, 1, DDS_SECS(1));

        if (num_events > 0) {
            messages_RcwsCommand msg;

            void *samples[1] = { &msg };
            dds_sample_info_t info[1];

            dds_return_t samples_read = dds_take(reader_, samples, info, 1, 1);

            if (samples_read > 0 && info[0].valid_data) {
                emit commandReceived(msg.pan, msg.tilt, msg.fire);
            }
        }
    }

    dds_waitset_detach(waitset, readcond);
    dds_delete(readcond);
    dds_delete(waitset);
}