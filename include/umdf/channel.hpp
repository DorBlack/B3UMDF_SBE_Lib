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
// Created by Fábio da Silva Santana on 29/10/2023.
//

#ifndef MARKET_DATA_CHANNEL_HPP
#define MARKET_DATA_CHANNEL_HPP

#include <memory>
#include <thread>

#include "io/isocket.hpp"
#include "umdf/iengine.hpp"
#include "memory/st_cache.hpp"

namespace b3::umdf
{
    template<typename Ty>
    struct channel_notification
    {
        std::function<void(std::shared_ptr<Ty>)> on_instrument_def;
        std::function<void(std::shared_ptr<Ty>)> on_snapshot;
        std::function<void(std::shared_ptr<Ty>)> on_incremental;
    };

    template<typename Socket,
                typename Buffer,
                typename Engine,
                typename Protocol, typename Config>
    class channel
    {
    private:

        using thread_ptr = std::shared_ptr<std::thread>;

        enum class SocketType
        {
            Instrument_def,
            Snapshot,
            Incremental_feed_a,
            Incremental_feed_b
        };

    public:
        channel(Config configuration, std::shared_ptr<channel_notification<Protocol>> notify) :
            _configuration(configuration) , _notify(notify)
        {
        }

        void start()
        {
            create_sockets();
            create_engine();

            _th_instrument_def = std::make_shared<std::thread>(([&](){
                _sock_instrument_def->start();
            }));

            _th_snapshot = std::make_shared<std::thread>([&](){
                _sock_snapshot->start();
            });

            _th_incremental_feed_a = std::make_shared<std::thread>([&](){
                _sock_incremental_feed_a->start();
            });

            _th_incremental_feed_b = std::make_shared<std::thread>([&](){
                _sock_incremental_feed_b->start();
            });

            _th_dispatch_incremental = std::make_shared<std::thread>([&]() {
                dispatch_incremental();
            });
        }

        void stop()
        {
            if(_sock_instrument_def)
            {
                _sock_instrument_def->stop();
            }
            if(_sock_snapshot)
            {
                _sock_snapshot->stop();
            }
            if(_sock_incremental_feed_a)
            {
                _sock_incremental_feed_a->stop();
            }
            if(_sock_incremental_feed_b)
            {
                _sock_incremental_feed_b->stop();
            }
        }

        void reset()
        {

        }

        ~channel()
        {

        }
    private:

        void create_sockets()
        {
            {
                auto notification = std::make_shared<io::socket::socket_notification<Buffer>>();
                notification->on_error = [&](std::error_code ec) {
                    on_error_socket(SocketType::Instrument_def, ec);
                };

                notification->on_msg_received = [&](auto buffer)
                {
                    on_instrument_def_msg_received(buffer);
                };
                _sock_instrument_def = Socket::create_socket(_configuration.instrument_def.interface,
                        _configuration.instrument_def.address,
                        _configuration.instrument_def.port, notification);
            }

            {
                auto notification = std::make_shared<io::socket::socket_notification<Buffer>>();
                notification->on_error = [&](std::error_code ec) {
                    on_error_socket(SocketType::Snapshot, ec);
                };

                notification->on_msg_received = [&](auto buffer)
                {
                    on_snapshot_msg_received(buffer);
                };

                _sock_snapshot = Socket::create_socket(_configuration.snapshot.interface,
                        _configuration.snapshot.address,
                        _configuration.snapshot.port, notification);
            }

            {
                auto notification = std::make_shared<io::socket::socket_notification<Buffer>>();
                notification->on_error = [&](std::error_code ec) {
                    on_error_socket(SocketType::Incremental_feed_a, ec);
                };

                notification->on_msg_received = [&](auto buffer)
                {
                    on_incremental_a_msg_received(buffer);
                };

                _sock_incremental_feed_a = Socket::create_socket(_configuration.feed_a.interface,
                        _configuration.feed_a.address,
                        _configuration.feed_a.port, notification);
            }

            {
                auto notification = std::make_shared<io::socket::socket_notification<Buffer>>();
                notification->on_error = [&](std::error_code ec) {
                    on_error_socket(SocketType::Incremental_feed_b, ec);
                };

                notification->on_msg_received = [&](auto buffer)
                {
                    on_incremental_b_msg_received(buffer);
                };

                _sock_incremental_feed_b = Socket::create_socket(_configuration.feed_b.interface,
                        _configuration.feed_b.address,
                        _configuration.feed_b.port, notification);
            }
        }

        void create_engine()
        {
                auto engine_notification = std::make_shared<b3::engine::engine_notification<Protocol>>();
                engine_notification->on_incremental([&](auto msg) {
                    _notify->on_incremental(msg);
                });

                engine_notification->on_snapshot([&](auto msg) {
                    _notify->on_snapshot(msg);
                });

                engine_notification->on_instrument_def([&](auto msg) {
                    _notify->on_instrument_def(msg);
                });

                engine_notification->on_notification([&](auto notify) {
                    on_engine_notification(notify);
                });

                _engine = std::make_shared<Engine>(engine_notification);
        }

        void on_engine_notification(engine::NotificationType notification) {
            switch (notification) {
            case engine::NotificationType::InstrumentDefinition:break;
            case engine::NotificationType::Snapshot:break;
            case engine::NotificationType::IncrementalTransition:break;
            case engine::NotificationType::Incremental:break;
            case engine::NotificationType::IncrementalGap:break;
            case engine::NotificationType::IncrementalReset:break;
            case engine::NotificationType::IncrementalFullReset:break;
            }
        }

        void on_instrument_def_msg_received(std::shared_ptr<Buffer> buffer)
        {
            _engine->instrument_def(buffer);
        }

        void on_snapshot_msg_received(std::shared_ptr<Buffer> buffer)
        {
            _engine->snapshot(buffer);
        }

        void on_incremental_a_msg_received(std::shared_ptr<Buffer> buffer)
        {
            _cache_feed_a->input(buffer);
        }

        void on_incremental_b_msg_received(std::shared_ptr<Buffer> buffer)
        {
            _cache_feed_b->input(buffer);
        }

        void on_error_socket([[maybe_unused]]SocketType socket, [[maybe_unused]]std::error_code &error)
        {
            switch (socket) {
            }
        }

        void dispatch_incremental()
        {
            bool feed_a_is_ok;
            bool feed_b_is_ok;

            while(_is_running)
            {
                auto msg1 = _cache_feed_a->try_get_next();

                if(!msg1)
                {
                    feed_a_is_ok = true;
                }
                else
                {
                    feed_a_is_ok = _engine->incremental(msg1.value());
                }

                auto msg2 = _cache_feed_b->try_get_next();

                if(!msg2) {
                    feed_b_is_ok = true;
                }
                else
                {
                    feed_b_is_ok = _engine->incremental(msg2.value());
                }

                if(!feed_a_is_ok && !feed_b_is_ok)
                {
                    _engine->incremental_reset();
                }
            }
        }

        thread_ptr _th_instrument_def;
        thread_ptr _th_snapshot;
        thread_ptr _th_incremental_feed_a;
        thread_ptr _th_incremental_feed_b;
        thread_ptr _th_dispatch_incremental;

        std::shared_ptr<Socket> _sock_instrument_def;
        std::shared_ptr<Socket> _sock_snapshot;
        std::shared_ptr<Socket> _sock_incremental_feed_a;
        std::shared_ptr<Socket> _sock_incremental_feed_b;
        std::shared_ptr<memory::st_cache<std::shared_ptr<Buffer>>> _cache_feed_a;
        std::shared_ptr<memory::st_cache<std::shared_ptr<Buffer>>> _cache_feed_b;
        std::shared_ptr<Engine> _engine;
        std::shared_ptr<channel_notification<Protocol>> _notify;

        Config _configuration;
        bool _is_running;
    };
}
#endif //MARKET_DATA_CHANNEL_HPP
