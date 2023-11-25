#ifndef __UDP_SOCKET_RECEIVER__
#define __UDP_SOCKET_RECEIVER__

#include <array>
#include <chrono>
#include <exception>
#include <memory>
#include <functional>
#include <string>
#include <iostream>
#include <thread>

#include "asio.hpp"
#include "memory/allocator.hpp"
#include "memory/buffer.hpp"
#include "types.h"

namespace io::socket
{


class udp_multicast
{
public:

    using notification_type = std::shared_ptr<io::socket::socket_notification<memory::buffer>>;
    using Buffer_Type = memory::buffer;

    udp_multicast(const asio::ip::address& __listen_address,
            const asio::ip::address& __multicast_address,
            const short __port,
            notification_type __notify)
            : _M_io_context(),
              _M_socket(_M_io_context),
              _M_notify(__notify),
              _M_multicast_address(__multicast_address)
    {
        // Create the socket so that multiple may be bound to the same address.
        asio::ip::udp::endpoint listen_endpoint(
                __listen_address, __port);
        _M_socket.open(listen_endpoint.protocol());
        _M_socket.set_option(asio::ip::udp::socket::reuse_address(true));
        _M_socket.bind(listen_endpoint);
        // Join the multicast group.
        _M_socket.set_option(
                asio::ip::multicast::join_group(_M_multicast_address));

    }

    ~udp_multicast()
    {
        std::error_code ec;
        _M_socket.close(ec);
        _M_io_context.stop();
    }
    void start()
    {
        _M_is_running = true;
        _M_thread = std::make_shared<std::thread>(
                [&]()
                {
                    while(_M_is_running)
                    {
                        do_receive();
                        _M_io_context.run();
                    }
                }
                );
    }

    bool is_running()
    {
        return _M_is_running;
    }

    void resume()
    {
        /*
        if(_M_is_paused)
        {
            _M_is_paused = false;
            _M_socket.set_option(asio::ip::multicast::join_group(_M_multicast_address));
        }*/
    }

    void leave_group()
    {
        /*
        _M_is_paused = true;
        _M_socket.set_option(asio::ip::multicast::leave_group(_M_multicast_address));
         */
    }
    void stop()
    {
        /*
        _M_is_running = false;
         */
    }

    static std::shared_ptr<udp_multicast> create_socket(const std::string& __interface,
            const std::string& __address,
            const short __port,
            notification_type __notify
    )
    {
        return std::make_shared<udp_multicast>(asio::ip::make_address(__interface),
                asio::ip::make_address(__address), __port, __notify);
    }

private:

    void do_receive()
    {
        _M_current_buffer = memory::allocator::make_shared_ptr<memory::buffer>(&_M_storage);
//        _M_current_buffer = std::make_shared<memory::buffer>();

        _M_socket.async_receive_from(
                asio::buffer(_M_current_buffer->data(), _M_current_buffer->capacity()),
                _M_receiver_endpoint,
                [this](std::error_code ec, std::size_t length) {
                    received(ec, length);
                }
        );
    }

    void received(std::error_code __ec, std::size_t __length)
    {
        if(__ec)
        {
            _M_notify->on_error(__ec);
        }
        else
        {
            clock_gettime(CLOCK_MONOTONIC, &_M_timer);
            _M_current_buffer->created_at((long)_M_timer.tv_nsec);
            _M_current_buffer->set_size(__length);
            _M_notify->on_msg_received(_M_current_buffer);
        }

        if(_M_is_running && !_M_is_paused)
        {
            do_receive();
        }
    }

    std::shared_ptr<std::thread> _M_thread;
    memory::allocator _M_storage;
    std::shared_ptr<Buffer_Type> _M_current_buffer;
    asio::io_context _M_io_context;
    asio::ip::udp::socket _M_socket;
    asio::ip::udp::endpoint _M_receiver_endpoint;
    bool _M_is_running = false;
    bool _M_is_paused = false;
    notification_type _M_notify;
    struct timespec _M_timer;
    asio::ip::address _M_multicast_address;
};
} // namespace io

#endif
