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
#include "types.h"
#include "memory/st_cache.hpp"
#include "channel_config.hpp"
#include "channel_engine.hpp"


namespace b3::umdf
{
    template<typename Socket>
    class channel
    {

    private:
        using thread_ptr = std::shared_ptr<std::thread>;
        using Buffer = memory::buffer;
        enum class SocketType
        {
            Instrument_def,
            Snapshot,
            Incremental_feed_a
        };

    public:
        channel(b3::channel_config configuration, std::shared_ptr<channel_notification> notify) :
                _M_configuration(configuration) , _M_notify(notify)
        {
        }

        void start()
        {
            _M_is_running = true;
            _M_cache_feed_a = std::make_shared<memory::st_cache<std::shared_ptr<Buffer>>>();

            create_sockets();

            _M_th_dispatch_incremental = std::make_shared<std::thread>([&]() {
                dispatch_incremental();
            });

        }

        void stop()
        {
            if(_M_sock_instrument_definition)
            {
                _M_sock_instrument_definition->stop();
            }
            if(_M_sock_snapshot)
            {
                _M_sock_snapshot->stop();
            }
            if(_M_sock_incremental_feed_a)
            {
                _M_sock_incremental_feed_a->stop();
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
                _M_sock_instrument_definition = Socket::create_socket(_M_configuration.instrument_def.interface,
                                                                      _M_configuration.instrument_def.address,
                                                                      _M_configuration.instrument_def.port, notification);
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

                _M_sock_snapshot = Socket::create_socket(_M_configuration.snapshot.interface,
                                                         _M_configuration.snapshot.address,
                                                         _M_configuration.snapshot.port, notification);
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

                _M_sock_incremental_feed_a = Socket::create_socket(_M_configuration.feed_a.interface,
                                                                   _M_configuration.feed_a.address,
                                                                   _M_configuration.feed_a.port, notification);
            }

            _M_sock_instrument_definition->start();
            _M_sock_snapshot->start();
            _M_sock_incremental_feed_a->start();
        }

        void on_instrument_def_msg_received(std::shared_ptr<Buffer> buffer)
        {
            using namespace ::b3::channel;
            auto ret = channel_engine::process_instrument_definition(buffer, _M_channel_status);
            const auto& result = ret.first;

            switch(result) {
                case B3MsgResult::discard:
                    _M_instrument_definition_container.clear();
                    break;
                case B3MsgResult::enqueue:
                    _M_instrument_definition_container.emplace_back(std::move(ret.second.value()));
                    break;
                case B3MsgResult::send:
                {
                    for(const auto& value : _M_instrument_definition_container)
                    {
                        _M_notify->on_security_def(value);
                    }
                    _M_instrument_definition_container.clear();
                    channel_engine::update_phase(_M_channel_status);
                    _M_sock_instrument_definition->leave_group();
                    std::cout << "Phase: Snapshot" << std::endl;
                    break;
                }
                case B3MsgResult::sequencer:
                case B3MsgResult::ok:
                case B3MsgResult::incremental_trans:
                case B3MsgResult::channel_reset:
                case B3MsgResult::incremental_reset:
                case B3MsgResult::gap:
                case B3MsgResult::new_seq_version:
                {
                    break;
                }
            };
        }

        void on_snapshot_msg_received(std::shared_ptr<Buffer> __buffer)
        {
            using namespace ::b3::channel;
            auto ret = channel_engine::proccess_snapshot(__buffer, _M_channel_status);
            B3MsgResult result = ret.first;
            switch(result) {
                    //aqui vamos mandar o carrousel e atualizar o status
                case B3MsgResult::discard:
                {
                    _M_snapshot_container.clear();
                    break;
                }
                case B3MsgResult::enqueue:
                {
                    _M_snapshot_container.emplace_back(std::move(ret.second.value()));
                    break;
                }
                case B3MsgResult::send:
                {
                    for(const auto& value : _M_snapshot_container)
                    {
                        _M_notify->on_snapshot(value);
                    }
                    _M_snapshot_container.clear();
                    channel_engine::update_phase(_M_channel_status);
                    std::cout << "Phase: Incremental" << std::endl;
                    break;
                }

                case B3MsgResult::ok:
                case B3MsgResult::new_seq_version:
                case B3MsgResult::incremental_trans:
                case B3MsgResult::incremental_reset:
                case B3MsgResult::channel_reset:
                case B3MsgResult::gap:
                case B3MsgResult::sequencer:
                    break;
            };
        }

        void on_incremental_a_msg_received(std::shared_ptr<Buffer> buffer)
        {
            _M_cache_feed_a->input(buffer);
        }

        void on_error_socket([[maybe_unused]]SocketType socket, [[maybe_unused]]std::error_code &error)
        {
            switch (socket) {
            }
        }

        void dispatch_incremental()
        {
            using namespace ::b3::channel;

            while(_M_is_running)
            {
                auto msg1 = _M_cache_feed_a->try_get_next();

                if(!msg1)
                {
                    continue;
                }

                auto ret = channel_engine::process_incremental(msg1.value(), _M_channel_status);
                B3MsgResult result = ret.first;
                switch(result) {

                    case B3MsgResult::send: {
                        _M_notify->on_incremental(ret.second.value());
                        break;
                    }
                    case B3MsgResult::discard:

                        break;
                    case B3MsgResult::gap:
                    {
                        _M_sock_snapshot->resume();
                        std::cout << "incremental gap" << std::endl;
                        break;
                    }
                    case B3MsgResult::new_seq_version:
                        std::cout << "incremental new seq version" << std::endl;
                        break;
                    case B3MsgResult::incremental_reset:
                        std::cout << "Incremental reset" << std::endl;
                        break;
                    case B3MsgResult::enqueue:
                    {
                        _M_incremental_container.emplace_back(std::move(ret.second.value()));
                        break;
                    }
                    case B3MsgResult::incremental_trans:
                    {

                        auto last_seqnum = _M_channel_status._M_incremental.seqnum;
                        const auto& current_seqnum = ret.second.value().b3header->get_sequence_number();
                        std::cout << "Incremental transition: queue: " << _M_incremental_container.size() << std::endl;
                        std::cout << "Incremental transition: last seqnum: " << last_seqnum << std::endl;
                        std::cout << "Incremental transition: current seqnum: " << current_seqnum << std::endl;

                        if(!_M_incremental_container.empty())
                        {
                            const auto& size = _M_incremental_container.size() ;

                            for(std::uint64_t i = 0; i < size; ++i)
                            {
                                const auto& value = _M_incremental_container[i];
                                const auto& seqnum = value.b3header->get_sequence_number();

                                if (last_seqnum >= seqnum) continue;

                                if(last_seqnum == seqnum - 1)
                                {
                                    _M_notify->on_incremental(value);
                                    ++last_seqnum;
                                }
                                else
                                {
                                    _M_channel_status._M_phase = Phase::Snapshot;
                                    std::cout << "Gap no carrousel " << std::endl;
                                    break;
                                }
                            }
                            _M_incremental_container.clear();

                            if(_M_channel_status._M_phase == Phase::Snapshot) break;
                        }


                        if (last_seqnum + 1 == current_seqnum)
                        {
                            _M_notify->on_incremental(ret.second.value());
                            ++last_seqnum;
                            _M_channel_status._M_phase = Phase::Incremental;
                            _M_sock_snapshot->leave_group();
                        }
                        else if( last_seqnum < current_seqnum)
                        {
                            std::cout << "Gap na transição " << std::endl;
                            std::cout << "Incremental transition: last seqnum: " << last_seqnum << std::endl;
                            std::cout << "Incremental transition: current seqnum: " << current_seqnum << std::endl;
                            std::cout << "Gap na transição " << std::endl;
                            _M_channel_status._M_phase = Phase::Snapshot;
                            break;
                        }
                        else
                        {
                            _M_channel_status._M_phase = Phase::Incremental;
                            _M_sock_snapshot->leave_group();
                        }
                        break;
                    }
                    case B3MsgResult::channel_reset:
                    {
                        std::cout << "Channel Reset" << std::endl;
                        break;
                    }
                }
            }
        }

        thread_ptr _M_th_dispatch_incremental;
        std::shared_ptr<Socket> _M_sock_instrument_definition;
        std::shared_ptr<Socket> _M_sock_snapshot;
        std::shared_ptr<Socket> _M_sock_incremental_feed_a;
        std::shared_ptr<memory::st_cache<std::shared_ptr<Buffer>>> _M_cache_feed_a;
        std::shared_ptr<channel_notification> _M_notify;
        b3::channel::channel_status _M_channel_status;
        b3::channel_config _M_configuration;
        bool _M_is_running;
        std::vector<b3::protocol::sbe::message> _M_instrument_definition_container;
        std::vector<b3::protocol::sbe::message> _M_snapshot_container;
        std::vector<b3::protocol::sbe::message> _M_incremental_container;
    };
}
#endif //MARKET_DATA_CHANNEL_HPP
