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
// Created by scien on 01/11/2023.
//

#ifndef B3MKTDATA_PCAP_FILE_SOCKET_HPP
#define B3MKTDATA_PCAP_FILE_SOCKET_HPP

#include "types.h"
#include "pcap_reader.h"
#include <ctime>
#include "time.h"

namespace io {

template<typename Buffer>
struct socket_pcap {
    using notification_type = std::shared_ptr<io::socket::socket_notification<Buffer>>;

    using Buffer_Type = Buffer;

    socket_pcap(std::string path, notification_type notify)
            :
            _path(path), _notify(notify)
    {

        PcapResults out;
        out.output = [&](const u_char* data, std::size_t size) {
            auto buffer = std::make_shared<Buffer>();
            std::memcpy(buffer->data(), data, size);
            clock_gettime(CLOCK_MONOTONIC, &_timer);
            buffer->set_size(size);
            buffer->created_at(_timer.tv_nsec);
            _notify->on_msg_received(buffer);
        };
        out.error = [&](auto error) {

        };
        _pcap_reader = std::make_shared<PcapReader>(_path, out);
    }

    static std::shared_ptr<socket_pcap<Buffer>>
    create_socket(std::string interface, [[maybe_unused]]std::string address,
            [[maybe_unused]]short port, notification_type notify)
    {
        return std::make_shared<socket_pcap>(address, notify);
    }

    void start()
    {
        _pcap_reader->start();
    }

    void stop()
    {
    }

private:
    std::string _path;
    notification_type _notify;
    std::shared_ptr<PcapReader> _pcap_reader;
    struct timespec _timer;
};
}
#endif //B3MKTDATA_PCAP_FILE_SOCKET_HPP
