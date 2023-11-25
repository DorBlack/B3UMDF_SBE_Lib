//
// Created by santana on 11/15/23.
//

#ifndef UMDF_B3_SBE_TYPES_H
#define UMDF_B3_SBE_TYPES_H

#include <memory>
#include <functional>
#include "b3/protocol/sbe_message.hpp"

namespace b3::umdf
{
    struct channel_notification {
        using ProtocolType = b3::protocol::sbe::message;
        std::function<void(const ProtocolType&)> on_incremental;
        std::function<void(const ProtocolType&)> on_snapshot;
        std::function<void(const ProtocolType&)> on_security_def;
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
