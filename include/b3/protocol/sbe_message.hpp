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
// Created by scien on 30/10/2023.
//

#ifndef B3MKTDATA_SBE_MESSAGE_HPP
#define B3MKTDATA_SBE_MESSAGE_HPP
#include <memory>
#include <variant>
#include <vector>

#include "b3_umdf_mbo_sbe/MessageHeader.h"
#include "b3_umdf_mbo_sbe/Order_MBO_50.h"
#include "b3_umdf_mbo_sbe/DeleteOrder_MBO_51.h"
#include "b3_umdf_mbo_sbe/SequenceReset_1.h"
#include "b3_umdf_mbo_sbe/Sequence_2.h"
#include "b3_umdf_mbo_sbe/SecurityStatus_3.h"
#include "b3_umdf_mbo_sbe/SecurityDefinition_4.h"
#include "b3_umdf_mbo_sbe/News_5.h"
#include "b3_umdf_mbo_sbe/SecurityGroupPhase_10.h"
#include "b3_umdf_mbo_sbe/EmptyBook_9.h"
#include "b3_umdf_mbo_sbe/ChannelReset_11.h"
#include "b3_umdf_mbo_sbe/OpeningPrice_15.h"
#include "b3_umdf_mbo_sbe/TheoreticalOpeningPrice_16.h"
#include "b3_umdf_mbo_sbe/ClosingPrice_17.h"
#include "b3_umdf_mbo_sbe/AuctionImbalance_19.h"
#include "b3_umdf_mbo_sbe/PriceBand_20.h"
#include "b3_umdf_mbo_sbe/QuantityBand_21.h"
#include "b3_umdf_mbo_sbe/HighPrice_24.h"
#include "b3_umdf_mbo_sbe/LowPrice_25.h"
#include "b3_umdf_mbo_sbe/LastTradePrice_27.h"
#include "b3_umdf_mbo_sbe/SnapshotFullRefresh_Header_30.h"
#include "b3_umdf_mbo_sbe/Order_MBO_50.h"
#include "b3_umdf_mbo_sbe/DeleteOrder_MBO_51.h"
#include "b3_umdf_mbo_sbe/MassDeleteOrders_MBO_52.h"
#include "b3_umdf_mbo_sbe/Trade_53.h"
#include "b3_umdf_mbo_sbe/ForwardTrade_54.h"
#include "b3_umdf_mbo_sbe/ExecutionSummary_55.h"
#include "b3_umdf_mbo_sbe/ExecutionStatistics_56.h"
#include "b3_umdf_mbo_sbe/TradeBust_57.h"
#include "b3_umdf_mbo_sbe/SnapshotFullRefresh_Orders_MBO_71.h"

using namespace b3::umdf::mbo::sbe;

#include <iostream>

namespace b3::protocol::sbe
{
    struct b3_header
    {
        b3_header(const char* data, std::size_t offset, std::size_t size) :
        _data(data), _offset(offset), _size(size)
        {}

        uint8_t get_channel_id() const
        {
            uint8_t value = 0x0;
            std::memcpy(&value, _data, sizeof(value));
            return value;
        }
        uint16_t get_sequence_version() const
        {
            uint16_t value = 0x0;
            std::memcpy(&value, _data + 2, sizeof(value));
            return value;
        }
        uint32_t get_sequence_number() const
        {
            uint32_t value = 0x0;
            std::memcpy(&value, _data + 4, sizeof(value));
            return value;
        }

        uint64_t get_sending_time() const
        {
            uint64_t value = 0x0;
            std::memcpy(&value, _data + 8, sizeof(value));
            return value;
        }

        static  std::uint32_t encoded_lenght()
        {
            return 0x10;
        }

    private:
        const char* _data;
        const std::size_t _offset;
        const std::size_t _size;
    };

    struct framing_header {

        framing_header(const char* data, std::size_t offset, std::size_t size) :
        _data(data),
        _offset(offset),
        _size(size)
        {

        }

        std::uint16_t message_length() {
            std::uint16_t  value = 0x00;
            std::memcpy(&value, _data + _offset, sizeof(value));
            return value;
        }

        std::uint16_t encoding_type() {
            std::uint16_t  value = 0x00;
            std::memcpy(&value, _data + _offset + 2, sizeof(value));
            return value;
        }

        static std::uint32_t encoded_length() {
            return 0x04;
        }

    private:
        const char* _data;
        const std::size_t _offset;
        const std::size_t _size;
    };
    struct sbe_message {
        std::unique_ptr<MessageHeader> header;
        std::variant<
                     SequenceReset_1,
                     Sequence_2,
                     SecurityStatus_3,
                     SecurityGroupPhase_10,
                     SecurityDefinition_4,
                     News_5,
                     EmptyBook_9,
                     ChannelReset_11,
                     OpeningPrice_15,
                     TheoreticalOpeningPrice_16,
                     ClosingPrice_17,
                     AuctionImbalance_19,
                     PriceBand_20,
                     QuantityBand_21,
                     HighPrice_24,
                     LowPrice_25,
                     LastTradePrice_27,
                     SnapshotFullRefresh_Header_30,
                     Order_MBO_50,
                     DeleteOrder_MBO_51,
                     MassDeleteOrders_MBO_52,
                     Trade_53,
                     ForwardTrade_54,
                     ExecutionSummary_55,
                     ExecutionStatistics_56,
                     TradeBust_57,
                     SnapshotFullRefresh_Orders_MBO_71
                     > body;
    };

    template<typename Buffer>
    struct message
    {

        template<typename Ty>
        static std::shared_ptr<message> create_message(std::shared_ptr<Ty> buffer)
        {
            return std::make_shared<message>(buffer);
        }

        message(std::shared_ptr<Buffer> buffer) : _buffer(buffer) {
            build_msg();
        }

        std::unique_ptr<b3_header> b3header;
        std::vector<std::shared_ptr<sbe_message>> body;

        long get_created_time_nano()
        {
            return _buffer->created;
        }


    private:

        void build_msg()
        {
            b3header = std::make_unique<b3_header>(_buffer->data(), 0, _buffer->size());

            std::size_t current_offset = b3_header::encoded_lenght();

            while(current_offset < _buffer->size())
            {

                 current_offset += framing_header::encoded_length();

                 auto msg = decoder_body(_buffer, current_offset);
                 body.emplace_back(msg);
            }
        }

        template<typename Ty>
        inline void decode_sbe_body(std::shared_ptr<sbe_message>& sbe_msg, std::shared_ptr<Buffer>& buffer, std::size_t& offset)
        {
            sbe_msg->body = Ty();
            auto ptr = std::get_if<Ty>(&sbe_msg->body);
            ptr->wrapForDecode(buffer->data(), offset, sbe_msg->header->blockLength(),
                    sbe_msg->header->actingVersion(), buffer->size());
            offset += ptr->decodeLength();
        }

        std::shared_ptr<sbe_message> decoder_body(std::shared_ptr<Buffer> buffer, std::size_t& offset)
        {
            auto sbe_msg = std::make_shared<sbe_message>();

            sbe_msg->header = std::make_unique<MessageHeader>();
            sbe_msg->header->wrap(buffer->data(), offset, 0, buffer->size());
            offset += sbe_msg->header->encodedLength();

            auto template_id = sbe_msg->header->templateId();
            switch (template_id) {
            case Order_MBO_50::SBE_TEMPLATE_ID: {
                decode_sbe_body<Order_MBO_50>(sbe_msg, buffer, offset);
                break;
            }
            case DeleteOrder_MBO_51::SBE_TEMPLATE_ID: {
                decode_sbe_body<DeleteOrder_MBO_51>(sbe_msg, buffer, offset);
                break;
            }
            case SequenceReset_1::SBE_TEMPLATE_ID: {
                decode_sbe_body<SequenceReset_1>(sbe_msg, buffer, offset);
                break;
            }
            case SnapshotFullRefresh_Header_30::SBE_TEMPLATE_ID: {
                decode_sbe_body<SnapshotFullRefresh_Header_30>(sbe_msg, buffer, offset);
                break;
            }
            case SecurityDefinition_4::SBE_TEMPLATE_ID: {
                decode_sbe_body<SecurityDefinition_4>(sbe_msg, buffer, offset);
                break;
            }
            case Sequence_2::SBE_TEMPLATE_ID: {
                decode_sbe_body<Sequence_2>(sbe_msg, buffer, offset);
                break;
            }
            case SecurityStatus_3::SBE_TEMPLATE_ID: {
                decode_sbe_body<SecurityStatus_3>(sbe_msg, buffer, offset);
                break;
            }
            case SecurityGroupPhase_10::SBE_TEMPLATE_ID: {
                decode_sbe_body<SecurityGroupPhase_10>(sbe_msg, buffer, offset);
                break;
            }
            case News_5::SBE_TEMPLATE_ID: {
                decode_sbe_body<News_5>(sbe_msg, buffer, offset);
                break;
            }
            case EmptyBook_9::SBE_TEMPLATE_ID: {
                decode_sbe_body<News_5>(sbe_msg, buffer, offset);
                break;
            }
            case ChannelReset_11::SBE_TEMPLATE_ID: {
                decode_sbe_body<ChannelReset_11>(sbe_msg, buffer, offset);
                break;
            }
            case OpeningPrice_15::SBE_TEMPLATE_ID: {
                decode_sbe_body<OpeningPrice_15>(sbe_msg, buffer, offset);
                break;
            }
            case TheoreticalOpeningPrice_16::SBE_TEMPLATE_ID: {
                decode_sbe_body<TheoreticalOpeningPrice_16>(sbe_msg, buffer, offset);
                break;
            }
            case ClosingPrice_17::SBE_TEMPLATE_ID: {
                decode_sbe_body<ClosingPrice_17>(sbe_msg, buffer, offset);
                break;
            }
            case AuctionImbalance_19::SBE_TEMPLATE_ID: {
                decode_sbe_body<AuctionImbalance_19>(sbe_msg, buffer, offset);
                break;
            }
            case PriceBand_20::SBE_TEMPLATE_ID: {
                decode_sbe_body<PriceBand_20>(sbe_msg, buffer, offset);
                break;
            }
            case QuantityBand_21::SBE_TEMPLATE_ID: {
                decode_sbe_body<QuantityBand_21>(sbe_msg, buffer, offset);
                break;
            }
            case HighPrice_24::SBE_TEMPLATE_ID: {
                decode_sbe_body<HighPrice_24>(sbe_msg, buffer, offset);
                break;
            }
            case LowPrice_25::SBE_TEMPLATE_ID: {
                decode_sbe_body<LowPrice_25>(sbe_msg,buffer,offset);
                break;
            }
            case LastTradePrice_27::SBE_TEMPLATE_ID: {
                decode_sbe_body<LastTradePrice_27>(sbe_msg,buffer,offset);
                break;
            }
            case MassDeleteOrders_MBO_52::SBE_TEMPLATE_ID: {
                decode_sbe_body<MassDeleteOrders_MBO_52>(sbe_msg,buffer,offset);
                break;
            }
            case Trade_53::SBE_TEMPLATE_ID: {
                decode_sbe_body<Trade_53>(sbe_msg,buffer,offset);
                break;
            }
            case ForwardTrade_54::SBE_TEMPLATE_ID: {
                decode_sbe_body<ForwardTrade_54>(sbe_msg,buffer,offset);
                break;
            }
            case ExecutionSummary_55::SBE_TEMPLATE_ID: {
                decode_sbe_body<ExecutionSummary_55>(sbe_msg,buffer,offset);
                break;
            }
            case ExecutionStatistics_56::SBE_TEMPLATE_ID: {
                decode_sbe_body<ExecutionStatistics_56>(sbe_msg,buffer,offset);
                break;
            }
            case TradeBust_57::SBE_TEMPLATE_ID: {
                decode_sbe_body<TradeBust_57>(sbe_msg,buffer,offset);
                break;
            }
            case SnapshotFullRefresh_Orders_MBO_71::SBE_TEMPLATE_ID: {
                decode_sbe_body<SnapshotFullRefresh_Orders_MBO_71>(sbe_msg,buffer,offset);
                break;
            }
            default:
            {
                throw "Process Message Error";
            }
            }
            return sbe_msg;
        }

        std::shared_ptr<Buffer> _buffer;
    };
}

#endif //B3MKTDATA_SBE_MESSAGE_HPP
