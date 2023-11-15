//
// Created by santana on 11/15/23.
//

#ifndef UMDF_B3_SBE_TYPES_H
#define UMDF_B3_SBE_TYPES_H

#include <memory>
#include <functional>

namespace b3::engine
{
    enum class NotificationType
    {
        InstrumentDefinition,
        Snapshot,
        Incremental,
        IncrementalGap,
        IncrementalReset,
        IncrementalFullReset
    };

    template<typename Protocol>
    struct engine_notification
    {
        std::function<void(std::shared_ptr<Protocol>)> on_incremental;
        std::function<void(std::shared_ptr<Protocol>)> on_snapshot;
        std::function<void(std::shared_ptr<Protocol>)> on_instrument_def;
        std::function<void(NotificationType)> on_notification;
    };
}

namespace b3::umdf
{
    template<typename Ty>
    struct channel_notification
    {
        std::function<void(std::shared_ptr<Ty>)> on_instrument_def;
        std::function<void(std::shared_ptr<Ty>)> on_snapshot;
        std::function<void(std::shared_ptr<Ty>)> on_incremental;
    };
}

namespace io::socket
{
    template<typename Buffer>
    struct socket_notification
    {
        std::function<void(std::shared_ptr<Buffer>)> on_msg_received;
        std::function<void(std::error_code)> on_error;
    };
}

#endif //UMDF_B3_SBE_TYPES_H
