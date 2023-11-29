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
#include "types.h"
#include "channel_config.hpp"
#include "protocol/b3_sbe.hpp"
#include "io/multicast_udp_receiver.h"

namespace b3::umdf
{
    class channel
    {
    private:
        enum class Phase {
            Initial,
            Stopped,
            InstrumentDef,
            Snapshot,
            IncrementalTrans,
            Incremental
        };

        struct carrousel_status {
            int32_t version = 0x00;
            int32_t seqnum = 0x00;
            uint32_t report = 0x00;
            uint32_t tot_report = 0x00;
        };

        struct incremental_status {
            int32_t version = 0x00;
            uint64_t seqnum = 0x00;
        };

        struct channel_status {
            carrousel_status instrument_definition;
            carrousel_status snapshot;
            incremental_status incremental;
            std::atomic<Phase> phase = {Phase::Initial};
        };
    public:
        struct channel_notification {
            std::function<void(const uint32_t, const SbeMessage&)> on_incremental;
            std::function<void(const uint32_t, const SbeMessage&)> on_snapshot;
            std::function<void(const uint32_t, const SbeMessage&)> on_security_def;
        };
    private:

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
            configure_sockets();
        }

        bool start()
        {
            if(_M_channel_status.phase == Phase::Initial ||
                    _M_channel_status.phase == Phase::Stopped)
            {
                auto ret = _M_sock_instrument_definition.join_to_group(_M_configuration.instrument_def.interface,
                                                            _M_configuration.instrument_def.address,
                                                            _M_configuration.instrument_def.port);
                if(!ret)
                {
                    return false;
                }
                _M_channel_status.phase = Phase::InstrumentDef;
                std::cout << "join to instrument group " << std::endl;
                _M_sock_instrument_definition.start();
                return true;
            }
            return false;
        }

        void stop()
        {
            _M_sock_instrument_definition.stop();
            _M_sock_snapshot.stop();
            _M_sock_incremental_feed_a.stop();
        }

        void reset()
        {
        }

        ~channel()
        {
        }
    private:

        void configure_sockets()
        {
            if(!_M_sock_instrument_definition.bind_to_interface(_M_configuration.instrument_def.interface))
            {
                std::cout << "error bind to interface instrument def " << _M_configuration.instrument_def.interface << std::endl;
            }
            _M_sock_instrument_definition.set_output([&](io::network::udp_packet packet) {
                on_instrument_def_msg_received(packet.data, ntohs(packet.udp->len));
            });
            if(!_M_sock_snapshot.bind_to_interface(_M_configuration.snapshot.interface))
            {
                std::cout << "error bind to interface snapshot" << _M_configuration.snapshot.interface << std::endl;
            }
            _M_sock_snapshot.set_output([&](io::network::udp_packet packet) {
                on_snapshot_msg_received(packet.data, ntohs(packet.udp->len));
            });

            if(!_M_sock_incremental_feed_a.bind_to_interface(_M_configuration.feed_a.interface))
            {
                std::cout << "error bind to interface feed a" << _M_configuration.feed_a.interface << std::endl;
            }
            _M_sock_incremental_feed_a.set_output([&](io::network::udp_packet packet) {
                on_incremental_a_msg_received(packet.data, ntohs(packet.udp->len));
            });
        }

        void on_instrument_def_msg_received(char *buffer, uint32_t len)
        {
            auto packet = b3_packet(buffer, len);

            auto ret = check_instrument_definition_packet(packet);

            switch(ret) {
                case CarrouselAction::discard:
                    _M_instrument_definition_container.clear();
                    break;
                case CarrouselAction::enqueue:
                {
                    auto tmp = std::make_shared<TempBuffer>();
                    memcpy(tmp->buffer, packet.data, packet.len);
                    tmp->len = packet.len;
                    _M_instrument_definition_container.emplace_back(tmp);
                    break;
                }
                case CarrouselAction::send:
                {
                    for(auto value : _M_instrument_definition_container)
                    {
                        auto b3_pkt = b3_packet(value->buffer, value->len);
                        auto tmp = b3_pkt;
                        while(next_sbe_message(&tmp))
                        {
                            if(tmp.current_sbe_packet->sbe_msg_hdr.template_id == SecurityDefinition_4::SBE_TEMPLATE_ID)
                            {
                                auto msg1 = get_sbe_message<SecurityDefinition_4>(&tmp);
                                std::cout << "send symbol: " << msg1.symbol() << std::endl;
                        //        _M_notify->on_security_def(value);
                            }
                        }
                    }
                    _M_instrument_definition_container.clear();
                    _M_sock_instrument_definition.leave_group(_M_configuration.instrument_def.interface,
                                                               _M_configuration.instrument_def.address);
                    _M_channel_status.phase = Phase::Snapshot;
                    _M_channel_status.instrument_definition.report = 0x00;
                    _M_channel_status.instrument_definition.tot_report = 0x00;
                    std::cout << "Phase: Snapshot" << std::endl;
                    break;
                }
                case CarrouselAction::ignore:
                    break;
            };
        }

        void on_snapshot_msg_received(char *__buffer, uint32_t __len)
        {
            /*
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
            };*/
        }

        void on_incremental_a_msg_received(char* __buffer, uint32_t __len)
        {
            /*
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
                        _M_sock_snapshot.leave_group();
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
                        _M_sock_snapshot.leave_group();
                    }
                    break;
                }
                case B3MsgResult::channel_reset:
                {
                    std::cout << "Channel Reset" << std::endl;
                    break;
                }
            }*/
        }

        void on_error_socket([[maybe_unused]]SocketType socket, [[maybe_unused]]std::error_code &error)
        {
        }

        static constexpr int CONTAINER_SIZE = 1500;

        struct TempBuffer
        {
            char buffer[CONTAINER_SIZE];
            uint32_t len;
        };

        std::shared_ptr<channel_notification> _M_notify;
        channel_status _M_channel_status;
        b3::channel_config _M_configuration;
        io::network::multicast_udp_receiver _M_sock_instrument_definition;
        io::network::multicast_udp_receiver _M_sock_snapshot;
        io::network::multicast_udp_receiver _M_sock_incremental_feed_a;
        std::vector<std::shared_ptr<TempBuffer>> _M_instrument_definition_container;
        std::vector<std::array<char, CONTAINER_SIZE>> _M_snapshot_container;
        std::vector<std::array<char, CONTAINER_SIZE>> _M_incremental_container;
    private:
        enum class CarrouselAction
        {
            discard,
            ignore,
            enqueue,
            send
        };

        enum class CarrouselResult
        {
            ok,
            new_seq_ver,
            discard,
            gap
        };

        inline CarrouselResult carrousel_check(carrousel_status& __status, const b3_packet& msg) {
            CarrouselResult ret = CarrouselResult::ok;
            if(__status.version != msg.header->sequence_version)
            {
                if(msg.header->sequence_number == 1)
                {
                    ret = CarrouselResult::new_seq_ver;
                    __status.version = msg.header->sequence_version;
                    __status.seqnum = 1;
                }
                else
                {
                    ret = CarrouselResult::discard;
                }
            }
            else if(msg.header->sequence_number - 1 >  __status.seqnum)
            {
                ret = CarrouselResult::gap;
            }
            else if (msg.header->sequence_number - 1 < __status.seqnum)
            {
                ret = CarrouselResult::discard;
            }
            else
            {
                ++__status.seqnum;
            }
            return ret;
        }

        inline CarrouselAction check_instrument_definition_packet(const b3_packet& packet)
        {
            CarrouselAction ret = CarrouselAction::send;

            if(_M_channel_status.phase != Phase::InstrumentDef)
            {
                return CarrouselAction::discard;
            }
            if(packet.current_sbe_packet->sbe_msg_hdr.template_id  == Sequence_2::SBE_TEMPLATE_ID)
            {
                return CarrouselAction::ignore;
            }

            auto b3ret = carrousel_check(_M_channel_status.instrument_definition, packet);
            switch (b3ret) {
                case CarrouselResult::ok: {

                    if(packet.current_sbe_packet->sbe_msg_hdr.template_id == SequenceReset_1::SBE_TEMPLATE_ID)
                    {
                        if(_M_channel_status.instrument_definition.report != _M_channel_status.instrument_definition.tot_report)
                        {
                            ret = CarrouselAction::discard;
                            _M_channel_status.instrument_definition.report = 0x00;
                            _M_channel_status.instrument_definition.tot_report = 0x00;
                        }
                        else
                        {
                            _M_channel_status.instrument_definition.report = 0x00;
                            _M_channel_status.instrument_definition.tot_report = 0x00;
                            ret = CarrouselAction::send;
                        }
                    }
                    else
                    {
                        auto tmp = packet;
                        while(next_sbe_message(&tmp))
                        {
                            ++(_M_channel_status.instrument_definition.report);
                        }
                        ret = CarrouselAction::enqueue;
                    }
                    break;
                }
                case CarrouselResult::gap: {
                    _M_channel_status.instrument_definition.version = 0x00;
                    _M_channel_status.instrument_definition.seqnum = 0x00;
                    ret = CarrouselAction::discard;
                    break;
                }
                case CarrouselResult::new_seq_ver: {
                    if(packet.current_sbe_packet->sbe_msg_hdr.template_id == SecurityDefinition_4::SBE_TEMPLATE_ID)
                    {
                        auto msg = get_sbe_message<SecurityDefinition_4>(&packet);
                        _M_channel_status.instrument_definition.tot_report = msg.totNoRelatedSym();
                        _M_channel_status.instrument_definition.report = 0x00;
                        auto tmp = packet;
                        while(next_sbe_message(&tmp))
                        {
                            ++(_M_channel_status.instrument_definition.report);
                        }
                        ret = CarrouselAction::enqueue;
                    }
                    else
                    {
                        std::cout << "msg estranha" << std::endl;
                    }

                    break;
                }
                case CarrouselResult::discard:
                    ret = CarrouselAction::discard;
                    break;
                default:
                    ret = CarrouselAction::discard;
                    break;
            }
            return ret;
        }
    };
}
#endif //MARKET_DATA_CHANNEL_HPP
