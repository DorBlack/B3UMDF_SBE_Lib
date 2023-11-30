//
// Created by santana on 11/15/23.
//

#ifndef UMDF_B3_SBE_TYPES_H
#define UMDF_B3_SBE_TYPES_H

#include <memory>
#include <functional>
#include <variant>

#include "b3/protocol/b3_umdf_mbo_sbe/SequenceReset_1.h"
#include "b3/protocol/b3_umdf_mbo_sbe/Sequence_2.h"
#include "b3/protocol/b3_umdf_mbo_sbe/SecurityStatus_3.h"
#include "b3/protocol/b3_umdf_mbo_sbe/SecurityGroupPhase_10.h"
#include "b3/protocol/b3_umdf_mbo_sbe/SecurityDefinition_4.h"
#include "b3/protocol/b3_umdf_mbo_sbe/News_5.h"
#include "b3/protocol/b3_umdf_mbo_sbe/EmptyBook_9.h"
#include "b3/protocol/b3_umdf_mbo_sbe/ChannelReset_11.h"
#include "b3/protocol/b3_umdf_mbo_sbe/OpeningPrice_15.h"
#include "b3/protocol/b3_umdf_mbo_sbe/TheoreticalOpeningPrice_16.h"
#include "b3/protocol/b3_umdf_mbo_sbe/ClosingPrice_17.h"
#include "b3/protocol/b3_umdf_mbo_sbe/AuctionImbalance_19.h"
#include "b3/protocol/b3_umdf_mbo_sbe/PriceBand_20.h"
#include "b3/protocol/b3_umdf_mbo_sbe/QuantityBand_21.h"
#include "b3/protocol/b3_umdf_mbo_sbe/HighPrice_24.h"
#include "b3/protocol/b3_umdf_mbo_sbe/LowPrice_25.h"
#include "b3/protocol/b3_umdf_mbo_sbe/LastTradePrice_27.h"
#include "b3/protocol/b3_umdf_mbo_sbe/SnapshotFullRefresh_Header_30.h"
#include "b3/protocol/b3_umdf_mbo_sbe/Order_MBO_50.h"
#include "b3/protocol/b3_umdf_mbo_sbe/DeleteOrder_MBO_51.h"
#include "b3/protocol/b3_umdf_mbo_sbe/MassDeleteOrders_MBO_52.h"
#include "b3/protocol/b3_umdf_mbo_sbe/Trade_53.h"
#include "b3/protocol/b3_umdf_mbo_sbe/ForwardTrade_54.h"
#include "b3/protocol/b3_umdf_mbo_sbe/ExecutionSummary_55.h"
#include "b3/protocol/b3_umdf_mbo_sbe/ExecutionStatistics_56.h"
#include "b3/protocol/b3_umdf_mbo_sbe/TradeBust_57.h"
#include "b3/protocol/b3_umdf_mbo_sbe/SnapshotFullRefresh_Orders_MBO_71.h"

using namespace b3::umdf::mbo::sbe;

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

class sbe_message
{
private:

};
#endif //UMDF_B3_SBE_TYPES_H
