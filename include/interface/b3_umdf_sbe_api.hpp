// Copyright (c)  2023-${current_year},  Fábio da Silva Santana
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
//

//
// Created by scien on 06/11/2023.
//

#ifndef B3MKTDATA_B3_UMDF_SBE_API_HPP
#define B3MKTDATA_B3_UMDF_SBE_API_HPP

#include "b3/protocol/sbe_message.hpp"
#include "memory/buffer.hpp"
#include "b3/channel_config.hpp"
#include "b3/channel_engine.hpp"
#include "io/pcap_file_socket.hpp"
#include "io/udp_socket_receiver.hpp"
#include "b3/channel.hpp"

#include <functional>

namespace b3::umdf::sbe {

struct channel_notification {
    using protocol_type = b3::protocol::sbe::message<memory::buffer>;
    std::function<void(std::shared_ptr<protocol_type>)> on_incremental;
    std::function<void(std::shared_ptr<protocol_type>)> on_snapshot;
    std::function<void(std::shared_ptr<protocol_type>)> on_security_def;
};

/*
struct backtest_channel {

    using buffer_type = memory::buffer;
    using socket_type = io::socket_pcap<buffer_type>;
    using channel_type = umdf::channel<socket_type,
                                            b3::umdf::umdf_b3_sbe_engine,
                                            b3::protocol::sbe::message,
                                            b3::engine::channel_config>;

    backtest_channel(b3::engine::channel_config& config, std::shared_ptr<channel_notification> notify) : _config(config),
    _notify(notify){}

    void start()
    {
       create_channel();
       _channel->start();
    }
private:
    void create_channel()
    {
        auto notify = std::make_shared<umdf::channel_notification<b3::protocol::sbe::message<buffer_type>>>();

        notify->on_instrument_def = [&](auto msg) {
           _notify->on_security_def(msg);
        };

        notify->on_snapshot = [&](auto msg) {
            _notify->on_snapshot(msg);
        };

        notify->on_incremental = [&](auto msg) {
            _notify->on_incremental(msg);
        };
        _channel = std::make_shared<channel_type>(_config, notify);
    }

    const b3::engine::channel_config& _config;
    std::shared_ptr<channel_type> _channel;
    std::shared_ptr<channel_notification> _notify;

};*/

struct multicast_channel {
    using buffer_type = memory::buffer;
    using socket_type = io::socket::udp_multicast;
    using channel_type = umdf::channel<socket_type,
                                       b3::umdf::umdf_b3_sbe_engine,
                                       b3::protocol::sbe::message,
                                       b3::engine::channel_config>;

    multicast_channel(b3::engine::channel_config& config, std::shared_ptr<channel_notification> notify) : _config(config),
          _notify(notify){}

    void start()
    {
        create_channel();
        _channel->start();
    }
  private:
    void create_channel()
    {
        auto notify = std::make_shared<umdf::channel_notification<b3::protocol::sbe::message<buffer_type>>>();

        notify->on_instrument_def = [&](auto msg) {
            _notify->on_security_def(msg);
        };

        notify->on_snapshot = [&](auto msg) {
            _notify->on_snapshot(msg);
        };

        notify->on_incremental = [&](auto msg) {
            _notify->on_incremental(msg);
        };
        _channel = std::make_shared<channel_type>(_config, notify);
    }

    const b3::engine::channel_config& _config;
    std::shared_ptr<channel_type> _channel;
    std::shared_ptr<channel_notification> _notify;
};

};

#endif //B3MKTDATA_B3_UMDF_SBE_API_HPP
