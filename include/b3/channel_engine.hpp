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
// Created by scien on 29/10/2023.
//

#ifndef MARKET_DATA_CHANNEL_ENGINE_HPP
#define MARKET_DATA_CHANNEL_ENGINE_HPP

#include "b3/protocol/sbe_message.hpp"
#include "types.h"

#include <utility>
#include <optional>
#include <atomic>

namespace b3::channel
{
    enum class Phase {
        InstrumentDef,
        Snapshot,
        IncrementalTrans,
        Incremental
    };

    enum class B3MsgResult {
        ok,
        discard,
        gap,
        incremental_reset,
        incremental_trans,
        ignore,
        send,
        sequencer,
        new_seq_version,
        channel_reset,
        enqueue
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
        carrousel_status _M_instrument_definition;
        carrousel_status _M_snapshot;
        incremental_status _M_incremental;
        std::atomic<Phase> _M_phase = {Phase::InstrumentDef };
    };


    class channel_engine
    {
        using ProtocolType = protocol::sbe::message;
        using BufferType_Sptr = std::shared_ptr<memory::buffer>;
        using ReturnType = std::pair<B3MsgResult, std::optional<ProtocolType>>;

    public:

        static void update_phase(channel_status& __ch_status)
        {
            const auto& phase = __ch_status._M_phase;
            switch (phase) {

                case Phase::InstrumentDef:
                    __ch_status._M_phase = Phase::Snapshot;
                    break;
                case Phase::Snapshot:
                    __ch_status._M_phase = Phase::IncrementalTrans;
                    break;
                case Phase::Incremental:
                case Phase::IncrementalTrans:
                    __ch_status._M_phase = Phase::Incremental;
                    break;
            }
        }

        static inline ReturnType  process_instrument_definition(BufferType_Sptr __buffer, channel_status& __ch_status)
        {
            if(__ch_status._M_phase != Phase::InstrumentDef)
            {
                return { B3MsgResult::ignore, std::nullopt };
            }

            auto msg = ProtocolType(__buffer);

            if(msg.body[0]->header->templateId() == Sequence_2::SBE_TEMPLATE_ID)
            {
                return {B3MsgResult::ignore, std::move(msg)};
            }

            auto b3ret = carrousel_check(__ch_status._M_instrument_definition, msg);

            switch (b3ret) {
                case B3MsgResult::ok: {
                    if(msg.body[0]->header->templateId() == SequenceReset_1::SBE_TEMPLATE_ID)
                    {
                        if(__ch_status._M_instrument_definition.report != __ch_status._M_instrument_definition.tot_report)
                        {
                            b3ret = B3MsgResult::discard;
                        }
                        else
                        {
                            __ch_status._M_instrument_definition.report = 0x00;
                            __ch_status._M_instrument_definition.tot_report = 0x00;
                            b3ret = B3MsgResult::send;
                        }
                    }
                    else
                    {
                        __ch_status._M_instrument_definition.report += msg.body.size();
                        b3ret = B3MsgResult::enqueue;
                    }
                    break;
                }
                case B3MsgResult::discard:
                case B3MsgResult::gap: {
                    __ch_status._M_instrument_definition.version = 0x00;
                    __ch_status._M_instrument_definition.seqnum = 0x00;
                    b3ret = B3MsgResult::discard;
                    break;
                }
                case B3MsgResult::new_seq_version: {
                    auto ptr = std::get_if<SecurityDefinition_4>(&msg.body[0]->body);
                    __ch_status._M_instrument_definition.tot_report = ptr->totNoRelatedSym();
                    __ch_status._M_instrument_definition.report = 0x00;
                    __ch_status._M_instrument_definition.report += msg.body.size();
                    b3ret = B3MsgResult::enqueue;
                    break;
                }
                default: break;
            }

            return {b3ret, std::move(msg)};
        }

        static inline ReturnType proccess_snapshot(BufferType_Sptr __buffer, channel_status& __ch_status) {

            if(__ch_status._M_phase != Phase::Snapshot)
            {
                return {B3MsgResult::ignore, std::nullopt};
            }

            auto msg = ProtocolType (__buffer);

            if(msg.body[0]->header->templateId() == Sequence_2::SBE_TEMPLATE_ID)
            {
                return {B3MsgResult::ignore, std::move(msg)};
            }

            auto b3ret = carrousel_check(__ch_status._M_snapshot, msg);

            switch (b3ret) {
                case B3MsgResult::gap:
                case B3MsgResult::discard: {
                    __ch_status._M_snapshot.report = 0x00;
                    __ch_status._M_snapshot.tot_report = 0x00;
                    break;
                }
                case B3MsgResult::ok: {
                    if(msg.body[0]->header->templateId() == SequenceReset_1::SBE_TEMPLATE_ID)
                    {
                        if(__ch_status._M_snapshot.report != __ch_status._M_snapshot.tot_report)
                        {
                            std::cout << "invalid snapshot faltando mensagem" << std::endl;
                            b3ret = B3MsgResult::discard;
                        }
                        else
                        {
                            __ch_status._M_snapshot.report = 0x00;
                            __ch_status._M_snapshot.tot_report = 0x00;
                            b3ret = B3MsgResult::send;
                        }
                    }
                    else
                    {
                        for(const auto& value : msg.body)
                        {
                            if(value->header->templateId() == SnapshotFullRefresh_Header_30::SBE_TEMPLATE_ID)
                            {
                                auto ptr = std::get_if<SnapshotFullRefresh_Header_30>(&(value->body));
                                __ch_status._M_incremental.seqnum = ptr->lastMsgSeqNumProcessed();
                                __ch_status._M_snapshot.tot_report = ptr->totNumReports();
                                ++__ch_status._M_snapshot.report;
                            }
                        }
                        b3ret = B3MsgResult::enqueue;
                    }
                    break;
                }
                case B3MsgResult::new_seq_version : {
                    __ch_status._M_snapshot.report = 0x00;

                    for(const auto& value : msg.body)
                    {
                        if(value->header->templateId() == SnapshotFullRefresh_Header_30::SBE_TEMPLATE_ID)
                        {
                            auto ptr = std::get_if<SnapshotFullRefresh_Header_30>(&value->body);
                            __ch_status._M_snapshot.tot_report = ptr->totNumReports();
                            __ch_status._M_incremental.seqnum = ptr->lastMsgSeqNumProcessed();
                            ++__ch_status._M_snapshot.report;
                        }
                    }
                    b3ret = B3MsgResult::enqueue;
                    break;
                }
                case B3MsgResult::incremental_reset:
                case B3MsgResult::incremental_trans:
                case B3MsgResult::channel_reset:
                case B3MsgResult::enqueue:
                case B3MsgResult::ignore:
                case B3MsgResult::send:
                case B3MsgResult::sequencer:
                    break;
            }

            return { b3ret, std::move(msg)};
        }

        static inline ReturnType process_incremental(BufferType_Sptr __buffer, channel_status& __ch_status) {

            if(__ch_status._M_phase == channel::Phase::InstrumentDef)
            {
                return {B3MsgResult::discard, std::nullopt};
            }

            auto msg = ProtocolType(__buffer);

            if(msg.body[0]->header->templateId() == Sequence_2::SBE_TEMPLATE_ID)
            {
                return {B3MsgResult::discard, std::move(msg)};
            }

            if(msg.body[0]->header->templateId() == SequenceReset_1::SBE_TEMPLATE_ID)
            {
                __ch_status._M_phase = channel::Phase::Snapshot;
                __ch_status._M_incremental.version = 0x00;
                __ch_status._M_incremental.seqnum= 0x00;
                return {B3MsgResult::incremental_reset, std::move(msg)};
            }

            if(msg.body[0]->header->templateId() == ChannelReset_11::SBE_TEMPLATE_ID)
            {
                __ch_status._M_phase = channel::Phase::InstrumentDef;
                __ch_status._M_incremental.version = 0x00;
                __ch_status._M_incremental.seqnum= 0x00;
                return {B3MsgResult::channel_reset, std::move(msg)};
            }

            if(__ch_status._M_phase == channel::Phase::Snapshot)
            {
                return {B3MsgResult::enqueue, std::move(msg)};
            }

            if(__ch_status._M_phase == channel::Phase::IncrementalTrans) {
               return {B3MsgResult::incremental_trans, std::move(msg)};
            }
            //first msg processed
            if(__ch_status._M_incremental.version == 0x00)
            {
                __ch_status._M_incremental.version = msg.b3header->get_sequence_version();
            }

            if(__ch_status._M_incremental.version != msg.b3header->get_sequence_version())
            {
                __ch_status._M_incremental.version = 0x00;
                __ch_status._M_incremental.seqnum = 0x00;
                __ch_status._M_phase = Phase::Snapshot;
                return {B3MsgResult::channel_reset, std::move(msg)};
            }

            auto seq_num = msg.b3header->get_sequence_number();

            if( seq_num - 1 == __ch_status._M_incremental.seqnum)
            {
                ++__ch_status._M_incremental.seqnum;
                return {B3MsgResult::send, std::move(msg)};
            }
            else if( seq_num - 1 > __ch_status._M_incremental.seqnum)
            {
                __ch_status._M_incremental.seqnum = 0x00;
                __ch_status._M_phase = Phase::Snapshot;
                return {B3MsgResult::gap, std::move(msg)};
            }
            else
            {
                return {B3MsgResult::discard, std::move(msg)};
            }
        }

    private:
        static inline B3MsgResult carrousel_check(channel::carrousel_status& __status,  const ProtocolType& msg) {

            B3MsgResult ret = B3MsgResult::ok;

            if(__status.version != msg.b3header->get_sequence_version())
            {
                if(msg.b3header->get_sequence_number() == 1)
                {
                    ret = B3MsgResult::new_seq_version;
                    __status.version = msg.b3header->get_sequence_version();
                    __status.seqnum = 1;
                }
                else
                {
                    ret = B3MsgResult::discard;
                }
            }
            else if(msg.b3header->get_sequence_number() - 1 >  __status.seqnum)
            {
                ret = B3MsgResult::gap;
            }
            else if (msg.b3header->get_sequence_number() - 1 < __status.seqnum)
            {
                ret = B3MsgResult::discard;
            }
            else
            {
                ++__status.seqnum;
            }

            return ret;
        }
    };
}
#endif //MARKET_DATA_CHANNEL_ENGINE_HPP
