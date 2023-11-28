//
// Created by santana on 11/15/23.
//

#ifndef UMDF_B3_SBE_TYPES_H
#define UMDF_B3_SBE_TYPES_H

#include <memory>
#include <functional>
#include "b3/protocol/sbe_message.hpp"

using SbeMessage = std::variant<
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
>;


namespace io::socket
{
    template<typename Buffer>
    struct socket_notification
    {
        std::function<void(std::shared_ptr<Buffer>)> on_msg_received;
        std::function<void(std::error_code)> on_error;
    };
}

#endif //UMDF_B3_SBE_TYPES_H
