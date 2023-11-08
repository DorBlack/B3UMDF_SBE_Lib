#ifndef __UDP_SOCKET_RECEIVER__
#define __UDP_SOCKET_RECEIVER__

#include <array>
#include <chrono>
#include <exception>
#include <memory>
#include <functional>
#include <string>
#include <iostream>

#include "asio.hpp"
#include "memory/allocator.hpp"

namespace io::socket
{


template< template<typename Ty> class Notification, typename Buffer>
class udp_multicast
{
public:

    using Buffer_Type = Buffer;

    udp_multicast(const asio::ip::address& listen_address,
            const asio::ip::address& multicast_address,
            const short port,
            std::shared_ptr<Notification<Buffer>> notify)
            : _io_context(),
              _socket(_io_context),
              _notify(notify)
    {
        // Create the socket so that multiple may be bound to the same address.
        asio::ip::udp::endpoint listen_endpoint(
                listen_address, port);
        _socket.open(listen_endpoint.protocol());
        _socket.set_option(asio::ip::udp::socket::reuse_address(true));
        _socket.bind(listen_endpoint);

        // Join the multicast group.
        _socket.set_option(
                asio::ip::multicast::join_group(multicast_address));
    }

    ~udp_multicast()
    {
        std::error_code ec;
        _socket.close(ec);
        _io_context.stop();
    }
    void start()
    {
        _is_running = true;
        do_receive();
        _io_context.run();
    }

    bool is_running()
    {
        return _is_running;
    }

    void stop()
    {
        _is_running = false;
        std::error_code ec;
        _socket.close(ec);
        _io_context.stop();
    }

    static std::shared_ptr<udp_multicast<Notification, Buffer>> create_socket(const std::string& interface,
            const std::string& address,
            const short port,
            std::shared_ptr<Notification<Buffer>> notify
    )
    {
        return std::make_shared<udp_multicast<Notification, Buffer>>(asio::ip::make_address(interface),
                asio::ip::make_address(address), port, notify);
    }

private:

    void do_receive()
    {
        _current_buffer = memory::allocate_shared_buffer(_storage);
        _socket.async_receive_from(
                asio::buffer(_current_buffer->data(), _current_buffer->capacity()),
                _receiver_endpoint,
                [this](std::error_code ec, std::size_t length) {
                    received(ec, length);
                }
        );
    }

    void received(std::error_code ec, std::size_t length)
    {
        if(ec)
        {
            _notify->on_error(ec);
        }
        else
        {
            _current_buffer->set_size(length);
            _notify->on_msg_received(_current_buffer);
        }

        if(is_running())
        {
            do_receive();
        }
    }

    memory::cb_allocator<Buffer> _storage;
    std::shared_ptr<Buffer> _current_buffer;
    asio::io_context _io_context;
    asio::ip::udp::socket _socket;
    asio::ip::udp::endpoint _receiver_endpoint;
    bool _is_running = false;
    std::shared_ptr<Notification<Buffer>> _notify;
};
} // namespace io

#endif