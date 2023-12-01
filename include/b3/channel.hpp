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
#include "io/multicast_udp_receiver.h"
#include "protocol/b3_message.hpp"

namespace b3::umdf
{
    using  namespace b3::protocol;
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

            channel_status() = default;

            channel_status(channel_status& __other) {
                instrument_definition = __other.instrument_definition;
                snapshot = __other.snapshot;
                incremental = __other.incremental;
                phase.store(__other.phase);
            }
            channel_status& operator=(channel_status& __other) {
                instrument_definition = __other.instrument_definition;
                snapshot = __other.snapshot;
                incremental = __other.incremental;
                phase.store(__other.phase);
                return *this;
            }
        };
    public:
        struct channel_notification {
            std::function<void(const sbe::sbe_message&)> on_incremental;
            std::function<void(const sbe::sbe_message&)> on_snapshot;
            std::function<void(const sbe::sbe_message&)> on_security_def;
        };
    public:
        channel(b3::channel_config configuration, std::shared_ptr<channel_notification> notify) :
                _M_configuration(configuration) , _M_notify(notify),
                _M_sock_instrument_definition(configuration.instrument_def.interface,
                                              configuration.instrument_def.address,
                                              configuration.instrument_def.port),
                _M_sock_snapshot(configuration.snapshot.interface,
                                 configuration.snapshot.address,
                                 configuration.snapshot.port),
                _M_sock_incremental_feed_a(configuration.feed_a.interface,
                                           configuration.feed_a.address,
                                           configuration.feed_a.port)
        {
            configure_sockets();
        }
        bool start()
        {
            if(_M_channel_status.phase == Phase::Initial ||
                    _M_channel_status.phase == Phase::Stopped)
            {
                auto ret = _M_sock_instrument_definition.join_to_group();
                if(!ret)
                {
                    return false;
                }
                _M_channel_status.phase = Phase::InstrumentDef;
                _M_sock_instrument_definition.start();
                _M_sock_snapshot.start();
                _M_sock_incremental_feed_a.start();
                return true;
            }
            return false;
        }
        void stop()
        {
            _M_sock_instrument_definition.stop();
            _M_sock_snapshot.stop();
            _M_sock_incremental_feed_a.stop();
            clean_status();
            _M_channel_status.phase = Phase::Stopped;
        }
        void reset()
        {
            stop();
            start();
        }
        ~channel()
        {
            stop();
        }
    private:
        void clean_status()
        {
            auto n = channel_status();
            _M_channel_status = n;
        }
        void configure_sockets()
        {
            _M_sock_instrument_definition.set_output([&](io::network::udp_packet packet) {
                on_instrument_def_msg_received(packet.data, ntohs(packet.udp->len) - 8, packet.timestamp_ns);
            });
            _M_sock_snapshot.set_output([&](io::network::udp_packet packet) {
                on_snapshot_msg_received(packet.data, ntohs(packet.udp->len) - 8, packet.timestamp_ns);
            });
            _M_sock_incremental_feed_a.set_output([&](io::network::udp_packet packet) {
                on_incremental_a_msg_received(packet.data, ntohs(packet.udp->len) - 8, packet.timestamp_ns);
            });
        }
        void on_instrument_def_msg_received(char *buffer, uint32_t len, uint64_t  created_time_ns)
        {
            auto packet = sbe::message(buffer, len, created_time_ns);
            packet.set_created_time(created_time_ns);
            auto ret = check_instrument_definition_packet(packet);
            switch(ret) {
                case CarrouselAction::discard:
                    _M_instrument_definition_container.clear();
                    break;
                case CarrouselAction::enqueue:
                {
                    auto tmp = std::make_shared<TempBuffer>();
                    memcpy(tmp->buffer, packet.data(), packet.size());
                    tmp->len = packet.size();
                    _M_instrument_definition_container.emplace_back(tmp);
                    break;
                }
                case CarrouselAction::send:
                {
                    for(auto value : _M_instrument_definition_container)
                    {
                        auto b3_pkt = sbe::message(value->buffer, value->len, created_time_ns);
                        auto tmp = b3_pkt;
                        do
                        {
                            _M_notify->on_security_def(*(tmp.current_sbe_msg));
                        }
                        while(tmp.has_next_sbe_msg());
                    }
                    _M_instrument_definition_container.clear();
                    _M_sock_instrument_definition.leave_group();
                    _M_sock_snapshot.join_to_group();
                    _M_sock_incremental_feed_a.join_to_group();
                    _M_channel_status.phase = Phase::Snapshot;
                    _M_channel_status.instrument_definition.report = 0x00;
                    _M_channel_status.instrument_definition.tot_report = 0x00;
                    std::cout << "Phase: Snapshot" << std::endl;
                    break;
                }
                case CarrouselAction::ignore:
                    break;
            }
        }
        void on_snapshot_msg_received(char *__buffer, uint32_t __len, uint64_t  created_time_ns)
        {
            auto packet = sbe::message(__buffer, __len, created_time_ns);
            packet.set_created_time(created_time_ns);
            auto ret = check_snapshot_packet(packet);
            switch(ret) {
                case CarrouselAction::discard:
                    _M_snapshot_container.clear();
                    break;
                case CarrouselAction::enqueue:
                {
                    auto tmp = std::make_shared<TempBuffer>();
                    memcpy(tmp->buffer, packet.data(), packet.size());
                    tmp->len = packet.size();
                    _M_snapshot_container.emplace_back(tmp);
                    break;
                }
                case CarrouselAction::send:
                {
                    for(auto value : _M_snapshot_container)
                    {
                        auto b3_pkt = sbe::message(value->buffer, value->len, created_time_ns);
                        auto tmp = b3_pkt;
                        do
                        {
                            _M_notify->on_security_def(*tmp.current_sbe_msg);
                        }
                        while(tmp.has_next_sbe_msg());
                    }
                    _M_snapshot_container.clear();
                    _M_sock_snapshot.leave_group();
                    _M_channel_status.phase = Phase::IncrementalTrans;
                    _M_channel_status.snapshot.report = 0x00;
                    _M_channel_status.snapshot.tot_report = 0x00;
                    std::cout << "Phase: Incremental" << std::endl;
                    break;
                }
                case CarrouselAction::ignore:
                    break;
            }
        }

        struct timespec _timer;
        void on_incremental_a_msg_received(char* __buffer, uint32_t __len, uint64_t created_time_ns)
        {
            auto packet = sbe::message(__buffer, __len, created_time_ns);
            packet.set_created_time(created_time_ns);
            auto result = check_incremental_packet(packet);
            switch (result) {
                case IncrementalAction::send:
                    do
                    {
                        //        auto msg1 = get_sbe_message<SecurityDefinition_4>(&tmp);
                        //        std::cout << "send symbol: " << msg1.symbol() << std::endl;
                        _M_notify->on_incremental(*packet.current_sbe_msg);
                    }
                    while(packet.has_next_sbe_msg());
                    break;
                case IncrementalAction::discard:
                case IncrementalAction::ignore:
                {
                    return;
                }
                case IncrementalAction::enqueue:
                {
                    auto tmp = std::make_shared<TempBuffer>();
                    memcpy(tmp->buffer, packet.data(), packet.size());
                    tmp->len = packet.size();
                    _M_incremental_container.emplace_back(tmp);
                    break;
                }
                case IncrementalAction::transition:
                {
                    for(auto value : _M_incremental_container)
                    {
                        auto last_seqnum = _M_channel_status.incremental.seqnum;
                        const auto& current_seqnum = packet.header.get_sequence_number();
                        std::cout << "Incremental transition: queue: " << _M_incremental_container.size() << std::endl;
                        std::cout << "Incremental transition: last seqnum: " << last_seqnum << std::endl;
                        std::cout << "Incremental transition: current seqnum: " << current_seqnum << std::endl;

                        if(!_M_incremental_container.empty())
                        {
                            const auto& size = _M_incremental_container.size() ;
                            for(std::uint64_t i = 0; i < size; ++i)
                            {
                                const auto& bf = _M_incremental_container[i];
                                auto b3_pkt = sbe::message(bf->buffer, bf->len, created_time_ns);
                                const auto& seqnum = b3_pkt.header.get_sequence_number();
                                if (last_seqnum >= seqnum) continue;
                                if(last_seqnum == seqnum - 1)
                                {
                                    auto tmp = b3_pkt;
                                    do
                                    {
                                        //        auto msg1 = get_sbe_message<SecurityDefinition_4>(&tmp);
                                        //        std::cout << "send symbol: " << msg1.symbol() << std::endl;
                                        _M_notify->on_incremental(*tmp.current_sbe_msg);
                                    }
                                    while(tmp.has_next_sbe_msg());
                                    ++last_seqnum;
                                }
                                else
                                {
                                    _M_channel_status.phase = Phase::Snapshot;
                                    std::cout << "Gap no carrousel " << std::endl;
                                    break;
                                }
                            }
                            _M_incremental_container.clear();

                            if(_M_channel_status.phase == Phase::Snapshot)
                            {
                                break;
                            }
                        }
                        if (last_seqnum + 1 == current_seqnum)
                        {
                            auto tmp = packet;
                            do
                            {
                                _M_notify->on_incremental(*tmp.current_sbe_msg);
                            }
                            while(tmp.has_next_sbe_msg());
                            ++last_seqnum;
                            _M_channel_status.phase = Phase::Incremental;
                            _M_sock_snapshot.leave_group();
                        }
                        else if( last_seqnum < current_seqnum)
                        {
                            std::cout << "Gap na transição " << std::endl;
                            std::cout << "Incremental transition: last seqnum: " << last_seqnum << std::endl;
                            std::cout << "Incremental transition: current seqnum: " << current_seqnum << std::endl;
                            std::cout << "Gap na transição " << std::endl;
                            _M_channel_status.phase = Phase::Snapshot;
                            break;
                        }
                        else
                        {
                            _M_channel_status.phase = Phase::Incremental;
                            _M_sock_snapshot.leave_group();
                        }
                        break;
                    }
                    _M_incremental_container.clear();
                    break;
                }
                case IncrementalAction::gap:
                {
                    std::cout << "Gap" << std::endl;
                    _M_channel_status.incremental.seqnum = 0x00;
                    _M_channel_status.phase = Phase::Snapshot;
                    _M_sock_snapshot.join_to_group();
                    std::cout << "Phase: Snapshot" << std::endl;
                    break;
                }
                case IncrementalAction::channel_reset:
                    std::cout << "channel reset" << std::endl;
                    //segundo a documentação tudo é invalidado então voltamos a fase de
                    //instrument definition
                    _M_sock_incremental_feed_a.leave_group();
                    _M_channel_status.incremental.version = 0x00;
                    _M_channel_status.incremental.seqnum = 0x00;
                    _M_channel_status.phase = Phase::InstrumentDef;
                    //Enviar mensagem para o cliente deletar tudo
                    _M_sock_instrument_definition.join_to_group();
                    break;
                case IncrementalAction::sequential_reset:
                {
                    std::cout << "sequential reset" << std::endl;
                    //segundo a documentação caso isso ocorra a versão é incrementada e voltamos
                    //para o snapshot;
                   _M_channel_status.incremental.version += 1;
                   _M_channel_status.incremental.seqnum = 0x00;
                   _M_channel_status.phase = Phase::Snapshot;
                   //enviar a mensagem para o cliente limpar tudo;
                   _M_sock_snapshot.join_to_group();
                   std::cout << "Phase: Snapshot" << std::endl;
                    break;
                }
            }
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
        std::vector<std::shared_ptr<TempBuffer>> _M_snapshot_container;
        std::vector<std::shared_ptr<TempBuffer>> _M_incremental_container;
    private:
        enum class CarrouselAction
        {
            discard,
            ignore,
            enqueue,
            send
        };
        enum class IncrementalAction{
            discard,
            ignore,
            enqueue,
            transition,
            send,
            gap,
            channel_reset,
            sequential_reset
        };
        enum class CarrouselResult
        {
            ok,
            new_seq_ver,
            discard,
            gap
        };
        inline CarrouselResult carrousel_check(carrousel_status& __status, const sbe::message& msg) {
            CarrouselResult ret = CarrouselResult::ok;
            if(__status.version != msg.header.get_sequence_version())
            {
                if(msg.header.get_sequence_number() == 1)
                {
                    ret = CarrouselResult::new_seq_ver;
                    __status.version = msg.header.get_sequence_version();
                    __status.seqnum = 1;
                }
                else
                {
                    ret = CarrouselResult::discard;
                }
            }
            else if(msg.header.get_sequence_number() - 1 >  __status.seqnum)
            {
                ret = CarrouselResult::gap;
            }
            else if (msg.header.get_sequence_number() - 1 < __status.seqnum)
            {
                ret = CarrouselResult::discard;
            }
            else
            {
                ++__status.seqnum;
            }
            return ret;
        }
        inline CarrouselAction check_instrument_definition_packet(sbe::message& packet)
        {
            CarrouselAction ret = CarrouselAction::send;
            if(_M_channel_status.phase != Phase::InstrumentDef)
            {
                return CarrouselAction::discard;
            }
            if(packet.current_sbe_msg->header->templateId() == Sequence_2::SBE_TEMPLATE_ID)
            {
                return CarrouselAction::ignore;
            }
            auto b3ret = carrousel_check(_M_channel_status.instrument_definition, packet);
            switch (b3ret) {
                case CarrouselResult::ok: {
                    if(packet.current_sbe_msg->header->templateId() == SequenceReset_1::SBE_TEMPLATE_ID)
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
                        do
                        {
                            ++(_M_channel_status.instrument_definition.report);
                        }
                        while(tmp.has_next_sbe_msg());
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
                    if(packet.current_sbe_msg->header->templateId() == SecurityDefinition_4::SBE_TEMPLATE_ID)
                    {
                        auto msg = std::get_if<SecurityDefinition_4>(&packet.current_sbe_msg->body);
                        _M_channel_status.instrument_definition.tot_report = msg->totNoRelatedSym();
                        _M_channel_status.instrument_definition.report = 0x00;
                        auto tmp = packet;
                        do
                        {
                            ++(_M_channel_status.instrument_definition.report);
                        }
                        while(tmp.has_next_sbe_msg());
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
        inline CarrouselAction check_snapshot_packet(sbe::message& packet) {
            if(_M_channel_status.phase != Phase::Snapshot)
            {
                return CarrouselAction::discard;
            }
            if(packet.current_sbe_msg->header->templateId()  == Sequence_2::SBE_TEMPLATE_ID)
            {
                return CarrouselAction::ignore;
            }
            auto b3ret = carrousel_check(_M_channel_status.instrument_definition, packet);
            auto ret = CarrouselAction::discard;
            switch (b3ret) {
                case CarrouselResult::gap:
                case CarrouselResult::discard: {
                    break;
                }
                case CarrouselResult::ok: {
                    if(packet.current_sbe_msg->header->templateId() == SequenceReset_1::SBE_TEMPLATE_ID)
                    {
                        if(_M_channel_status.snapshot.report != _M_channel_status.snapshot.tot_report)
                        {
                            std::cout << "invalid snapshot faltando mensagem" << std::endl;
                            ret = CarrouselAction::discard;
                        }
                        else
                        {
                            ret = CarrouselAction::send;
                        }
                    }
                    else
                    {
                        auto tmp = packet;
                        do
                        {
                            if(tmp.current_sbe_msg->header->templateId() == SnapshotFullRefresh_Header_30::SBE_TEMPLATE_ID)
                            {
                                auto v = std::get_if<SnapshotFullRefresh_Header_30>(&tmp.current_sbe_msg->body);
                                _M_channel_status.incremental.seqnum = v->lastMsgSeqNumProcessed();
                                _M_channel_status.snapshot.tot_report = v->totNumReports();
                                ++_M_channel_status.snapshot.report;
                            }
                        }
                        while(tmp.has_next_sbe_msg());
                        ret = CarrouselAction::enqueue;
                    }
                    break;
                }
                case CarrouselResult::new_seq_ver: {
                    _M_channel_status.snapshot.report = 0x00;
                    auto tmp = packet;
                    do
                    {
                        if(packet.current_sbe_msg->header->templateId() == SnapshotFullRefresh_Header_30::SBE_TEMPLATE_ID)
                        {
                            auto v = std::get_if<SnapshotFullRefresh_Header_30>(&tmp.current_sbe_msg->body);
                            _M_channel_status.incremental.seqnum = v->lastMsgSeqNumProcessed();
                            _M_channel_status.snapshot.tot_report = v->totNumReports();
                            ++_M_channel_status.snapshot.report;
                        }
                    }
                    while(tmp.has_next_sbe_msg());
                    ret = CarrouselAction::enqueue;
                    break;
                }
            }
            return ret;
        }
        inline IncrementalAction check_incremental_packet(sbe::message& __packet) {
            if(_M_channel_status.phase == channel::Phase::InstrumentDef)
            {
                return  IncrementalAction::discard;
            }
            if(__packet.current_sbe_msg->header->templateId() == Sequence_2::SBE_TEMPLATE_ID)
            {
                return  IncrementalAction::discard;
            }
            if(__packet.current_sbe_msg->header->templateId() == SequenceReset_1::SBE_TEMPLATE_ID)
            {
                return IncrementalAction::sequential_reset;
            }
            if(__packet.current_sbe_msg->header->templateId() == ChannelReset_11::SBE_TEMPLATE_ID)
            {
                return IncrementalAction::channel_reset;
            }
            if(_M_channel_status.phase == channel::Phase::Snapshot)
            {
                return IncrementalAction::enqueue;
            }
            if(_M_channel_status.phase == channel::Phase::IncrementalTrans) {
                return IncrementalAction::transition;
            }
            //first msg processed
            if(_M_channel_status.incremental.version == 0x00)
            {
                _M_channel_status.incremental.version = __packet.header.get_sequence_version();
            }
            if(_M_channel_status.incremental.version != __packet.header.get_sequence_version())
            {
                return IncrementalAction::sequential_reset;
            }
            auto seq_num = __packet.header.get_sequence_number();
            if( seq_num - 1 == _M_channel_status.incremental.seqnum)
            {
                ++_M_channel_status.incremental.seqnum;
                return IncrementalAction::send;
            }
            else if( seq_num - 1 > _M_channel_status.incremental.seqnum)
            {
                return IncrementalAction::gap;
            }
            else
            {
                return IncrementalAction::discard;
            }
        }
    };
}
#endif //MARKET_DATA_CHANNEL_HPP
