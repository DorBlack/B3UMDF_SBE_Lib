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
// Created by scien on 05/11/2023.
//

#include "gtest/gtest.h"

#include "umdf/channel.hpp"
#include "b3/protocol/sbe_message.hpp"
#include "memory/buffer.hpp"
#include "io/udp_socket_receiver.hpp"
#include "io/pcap_file_socket.hpp"
#include "config/channel_config.hpp"
#include "umdf/channel_engine.hpp"



struct Sock {
    virtual void start() = 0;
    virtual void stop() = 0;
    virtual bool is_running() = 0;
    static std::shared_ptr<Sock> create_socket(const std::string& interface,
            const std::string& address,
            const short port,
            std::shared_ptr<io::socket::socket_notification<memory::buffer>> notify
    ) {
        return nullptr;
    }
};

struct EngineMock
{

};

using Buffer = memory::buffer;
using Socket = socket_pcap<Buffer>;
using Engine = b3::umdf::umdf_b3_sbe_engine<Buffer>;
using Protocol = b3::protocol::sbe::message<Buffer>;
using Channel = b3::umdf::channel<Socket, Buffer, Engine, Protocol, b3::engine::channel_config>;

TEST(ChannelTest, Create)
{
    b3::engine::channel_config config;

    auto notification = std::make_shared<b3::umdf::channel_notification<Protocol>>();
    notification->on_incremental = [&](auto msg) {};
    notification->on_instrument_def = [&](auto msg) {};
    notification->on_snapshot = [&](auto msg){};

    std::shared_ptr<Channel> ch = std::make_shared<Channel>(config, notification);
  //  ch->start();
}