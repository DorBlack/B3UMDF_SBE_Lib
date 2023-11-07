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
#include "iengine.hpp"

namespace b3::umdf
{

template<typename Buffer>
struct umdf_b3_sbe_engine {

    enum class Phase {
        InstrumentDef,
        Snapshot,
        Incremental
    };

    enum class B3SeqNumResult {
        Ok,
        Discard,
        Gap,
        NewSeqVersion
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

    struct status {
        carrousel_status instrument_def;
        carrousel_status snapshot;
        incremental_status incremental;
        Phase _phase = Phase::InstrumentDef;
    };

    using protocol_type = protocol::sbe::message<Buffer>;
    using notify_type = engine::engine_notification<protocol_type>;
    using carrousel_container = std::vector<std::shared_ptr<protocol_type>>;


    umdf_b3_sbe_engine(std::shared_ptr<notify_type> notify) : _notify(notify) {

    }

    void instrument_def(std::shared_ptr<Buffer> buffer) {
        _instrumental(buffer);
    }

    void snapshot(std::shared_ptr<Buffer> buffer) {
        if(_current_status._phase != Phase::Snapshot)
        {
            return;
        }
        build_snapshot_carrousel(buffer);
    }

    bool incremental(std::shared_ptr<Buffer> buffer) {
        return _incremental(buffer);
    }

    void incremental_reset() {

    }

private:

    void _instrumental(std::shared_ptr<Buffer> buffer)
    {
        if(_current_status._phase != Phase::InstrumentDef)
        {
            return;
        }

        std::shared_ptr<protocol_type> current_msg = nullptr;
        current_msg = protocol_type::create_message(buffer);
        auto b3ret = carrousel_check(_current_status.instrument_def, current_msg);

        switch (b3ret) {
        case B3SeqNumResult::Ok: {
            if(current_msg->body[0]->header->templateId() == SequenceReset_1::SBE_TEMPLATE_ID)
            {
                if(_current_status.instrument_def.report != _current_status.instrument_def.tot_report)
                {
                    //TODO Error
                }
                for(auto value : _storage_instrument_def)
                {
                    _notify->on_instrument_def(value);
                }
            }
            else
            {
                for(auto msg : current_msg->body)
                {
                    if(msg->header->templateId() == SecurityDefinition_4::SBE_TEMPLATE_ID)
                    {
                        auto ptr = std::get_if<SecurityDefinition_4>(&(msg->body));
                        _current_status.incremental.seqnum = ptr->totNoRelatedSym();
                    }

                    ++_current_status.snapshot.report;
                }
            }
            break;
        }
        case B3SeqNumResult::Gap: {
            _storage_instrument_def.clear();
            _current_status.instrument_def.version = 0x00;
            _current_status.instrument_def.seqnum = 0x00;
        }
        case B3SeqNumResult::NewSeqVersion: {
            _storage_instrument_def.clear();
            if(current_msg->body[0]->header->templateId() != SecurityDefinition_4::SBE_TEMPLATE_ID)
            {
                //TODO: Error a primeira mesagem deveria ser essa
            }
            auto ptr = std::get_if<SecurityDefinition_4>(&current_msg->body[0]->body);
            _current_status.instrument_def.tot_report = ptr->totNoRelatedSym();
            _current_status.instrument_def.report = 0x00;

            for(auto msg : current_msg->body)
            {
                ++_current_status.instrument_def.report;
            }

            break;
        }
        default: break;
        }
    }

    void build_snapshot_carrousel(std::shared_ptr<Buffer> buffer) {

        std::shared_ptr<protocol_type> current_msg = nullptr;
        current_msg = protocol_type::create_message(buffer);
        auto b3ret = carrousel_check(_current_status.snapshot, current_msg);

        switch (b3ret) {
        case B3SeqNumResult::Discard: {
            _storage_snapshot.clear();
            _current_status.snapshot.report = 0x00;
            _current_status.snapshot.tot_report = 0x00;
            break;
        }
        case B3SeqNumResult::Ok: {

            if(current_msg->body[0]->header->templateId() == SequenceReset_1::SBE_TEMPLATE_ID)
            {
                if(_current_status.snapshot.report != _current_status.snapshot.tot_report)
                {
                    //TODO Error
                }
                for(auto value : _storage_snapshot)
                {
                    _notify->on_snapshot(value);
                }
            }
            else
            {
                for(auto msg : current_msg->body)
                {
                    if(msg->header->templateId() == SnapshotFullRefresh_Header_30::SBE_TEMPLATE_ID)
                    {
                        auto ptr = std::get_if<SnapshotFullRefresh_Header_30>(&(msg->body));
                        _current_status.incremental.seqnum = ptr->lastMsgSeqNumProcessed();
                    }

                    ++_current_status.snapshot.report;
                }
            }

           break;
        }
        case B3SeqNumResult::NewSeqVersion: {
            _storage_snapshot.clear();
            if(current_msg->body[0]->header->templateId() != SnapshotFullRefresh_Header_30::SBE_TEMPLATE_ID)
            {
                //TODO: Error a primeira mesagem deveria ser essa
            }

            auto ptr = std::get_if<SnapshotFullRefresh_Header_30>(&current_msg->body[0]->body);
            _current_status.snapshot.tot_report = ptr->totNumReports();
            _current_status.snapshot.report = 0x00;
            _current_status.incremental.seqnum = ptr->lastMsgSeqNumProcessed();

            for(auto msg : current_msg->body)
            {
                ++_current_status.snapshot.report;
            }
            break;
        }
        case B3SeqNumResult::Gap: {
            _storage_snapshot.clear();
            _current_status.snapshot.report = 0x00;
            _current_status.snapshot.tot_report = 0x00;
            break;
        }
        }
    }

    bool _incremental(std::shared_ptr<Buffer> buffer) {
        auto ret = true;
        if(_current_status._phase != Phase::Incremental)
        {
            return ret;
        }
        std::shared_ptr<protocol_type> current_msg = nullptr;
        current_msg = protocol_type::create_message(buffer);
        //first msg processed
        if(_current_status.incremental.version == 0x00)
        {
            _current_status.incremental.version = current_msg->b3header->get_sequence_version();
        }

        if(_current_status.incremental.version != current_msg->b3header->get_sequence_version())
        {

        }

        if(current_msg->body[0]->header->templateId() == Sequence_2::SBE_TEMPLATE_ID)
        {
            return ret;
        }

        auto b3ret = incremental_check(current_msg);

        switch (b3ret) {
        case B3SeqNumResult::Ok: {
            _notify->on_incremental(current_msg);
            break;
        }
        case B3SeqNumResult::Discard: {
            ret = true;
            break;
        }
        case B3SeqNumResult::Gap: {
                ret = false;
            break;
        }
        }
        return ret;
    }

    B3SeqNumResult incremental_check(std::shared_ptr<protocol_type> msg) {

        auto seq_num = msg->b3header->get_sequence_number();

        B3SeqNumResult ret = B3SeqNumResult::Discard;

        if( seq_num - 1 == _current_status.incremental.seqnum)
        {
            ++_current_status.incremental.seqnum;
            ret = B3SeqNumResult::Ok;
        }
        else if( seq_num - 1 > _current_status.incremental.seqnum)
        {
            ret = B3SeqNumResult::Gap;
        }

        return ret;
    }

    B3SeqNumResult carrousel_check(carrousel_status& status,  std::shared_ptr<protocol_type> msg) {

        B3SeqNumResult ret = B3SeqNumResult::Ok;

        if(status.version != msg->b3header->get_sequence_version())
        {
            if(msg->b3header->get_sequence_number() == 1)
            {
                ret = B3SeqNumResult::NewSeqVersion;
                status.version = msg->b3header->get_sequence_version();
                status.seqnum = 1;
            }
            else
            {
                ret = B3SeqNumResult::Discard;
            }
        }
        else if(msg->b3header->get_sequence_number() - 1 >  status.seqnum)
        {
            ret = B3SeqNumResult::Gap;
        }
        else if (msg->b3header->get_sequence_number() - 1 < status.seqnum)
        {
            ret = B3SeqNumResult::Discard;
        }

        return ret;
    }

    status _current_status;
    std::shared_ptr<notify_type> _notify;
    carrousel_container _storage_snapshot;
    carrousel_container _storage_instrument_def;
};
}

#endif //MARKET_DATA_CHANNEL_ENGINE_HPP
