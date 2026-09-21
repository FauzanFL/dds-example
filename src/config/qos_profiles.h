#pragma once
#include <dds/dds.hpp>

namespace QoSProfile {
// --- PUBLISHER QOS (Untuk menyebarkan data kapal) ---
inline dds::pub::qos::DataWriterQos TelemetryPub(const dds::pub::Publisher& pub) {
    return pub.default_datawriter_qos()
    << dds::core::policy::Reliability::BestEffort()
    << dds::core::policy::Durability::Volatile()
    << dds::core::policy::History::KeepLast(1);
}

// --- SUBSCRIBER QOS (Untuk mendengarkan komando/status) ---
inline dds::sub::qos::DataReaderQos CommandSub(const dds::sub::Subscriber& sub) {
    return sub.default_datareader_qos()
    << dds::core::policy::Reliability::Reliable()
    << dds::core::policy::Durability::TransientLocal()
    << dds::core::policy::History::KeepLast(10);
}

inline dds::sub::qos::DataReaderQos StatusSub(const dds::sub::Subscriber& sub) {
    return sub.default_datareader_qos()
    << dds::core::policy::Reliability::Reliable()
    << dds::core::policy::Durability::Volatile()
    << dds::core::policy::History::KeepLast(1);
}
}