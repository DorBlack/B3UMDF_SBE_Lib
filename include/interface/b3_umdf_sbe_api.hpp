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

#include "b3/channel_config.hpp"
#include "io/udp_socket_receiver.hpp"
#include "b3/channel.hpp"

namespace b3::umdf::sbe {

class multicast_channel {
public:
    using SocketType = io::socket::udp_multicast;
    using ChannelType = b3::umdf::channel<SocketType>;

    multicast_channel(b3::channel_config& __config,
                      std::shared_ptr<channel_notification> __notify) :
                      _M_config(__config),
                      _M_notify(__notify){}

    void start()
    {
        create_channel();
        _M_channel->start();
    }
private:
    void create_channel()
    {
        _M_channel = std::make_shared<ChannelType>(_M_config, _M_notify);
    }

    const b3::channel_config& _M_config;
    std::shared_ptr<ChannelType> _M_channel;
    std::shared_ptr<channel_notification> _M_notify;
};
}
#endif //B3MKTDATA_B3_UMDF_SBE_API_HPP
