/* Generated SBE (Simple Binary Encoding) message codec */
#ifndef _B3_UMDF_MBO_SBE_MDENTRYTYPE_CXX_H_
#define _B3_UMDF_MBO_SBE_MDENTRYTYPE_CXX_H_

#if !defined(__STDC_LIMIT_MACROS)
#  define __STDC_LIMIT_MACROS 1
#endif

#include <cstdint>
#include <iomanip>
#include <limits>
#include <ostream>
#include <stdexcept>
#include <sstream>
#include <string>

#define SBE_NULLVALUE_INT8 (std::numeric_limits<std::int8_t>::min)()
#define SBE_NULLVALUE_INT16 (std::numeric_limits<std::int16_t>::min)()
#define SBE_NULLVALUE_INT32 (std::numeric_limits<std::int32_t>::min)()
#define SBE_NULLVALUE_INT64 (std::numeric_limits<std::int64_t>::min)()
#define SBE_NULLVALUE_UINT8 (std::numeric_limits<std::uint8_t>::max)()
#define SBE_NULLVALUE_UINT16 (std::numeric_limits<std::uint16_t>::max)()
#define SBE_NULLVALUE_UINT32 (std::numeric_limits<std::uint32_t>::max)()
#define SBE_NULLVALUE_UINT64 (std::numeric_limits<std::uint64_t>::max)()

namespace b3 {
namespace umdf {
namespace mbo {
namespace sbe {

class MDEntryType
{
public:
    enum Value
    {
        BID = static_cast<char>(48),
        OFFER = static_cast<char>(49),
        TRADE = static_cast<char>(50),
        INDEX_VALUE = static_cast<char>(51),
        OPENING_PRICE = static_cast<char>(52),
        CLOSING_PRICE = static_cast<char>(53),
        SETTLEMENT_PRICE = static_cast<char>(54),
        SESSION_HIGH_PRICE = static_cast<char>(55),
        SESSION_LOW_PRICE = static_cast<char>(56),
        EXECUTION_STATISTICS = static_cast<char>(57),
        IMBALANCE = static_cast<char>(65),
        TRADE_VOLUME = static_cast<char>(66),
        OPEN_INTEREST = static_cast<char>(67),
        EMPTY_BOOK = static_cast<char>(74),
        SECURITY_TRADING_STATE_PHASE = static_cast<char>(99),
        PRICE_BAND = static_cast<char>(103),
        QUANTITY_BAND = static_cast<char>(104),
        COMPOSITE_UNDERLYING_PRICE = static_cast<char>(68),
        EXECUTION_SUMMARY = static_cast<char>(115),
        VOLATILITY_PRICE = static_cast<char>(118),
        TRADE_BUST = static_cast<char>(117),
        NULL_VALUE = static_cast<char>(0)
    };

    static MDEntryType::Value get(const char value)
    {
        switch (value)
        {
            case static_cast<char>(48): return BID;
            case static_cast<char>(49): return OFFER;
            case static_cast<char>(50): return TRADE;
            case static_cast<char>(51): return INDEX_VALUE;
            case static_cast<char>(52): return OPENING_PRICE;
            case static_cast<char>(53): return CLOSING_PRICE;
            case static_cast<char>(54): return SETTLEMENT_PRICE;
            case static_cast<char>(55): return SESSION_HIGH_PRICE;
            case static_cast<char>(56): return SESSION_LOW_PRICE;
            case static_cast<char>(57): return EXECUTION_STATISTICS;
            case static_cast<char>(65): return IMBALANCE;
            case static_cast<char>(66): return TRADE_VOLUME;
            case static_cast<char>(67): return OPEN_INTEREST;
            case static_cast<char>(74): return EMPTY_BOOK;
            case static_cast<char>(99): return SECURITY_TRADING_STATE_PHASE;
            case static_cast<char>(103): return PRICE_BAND;
            case static_cast<char>(104): return QUANTITY_BAND;
            case static_cast<char>(68): return COMPOSITE_UNDERLYING_PRICE;
            case static_cast<char>(115): return EXECUTION_SUMMARY;
            case static_cast<char>(118): return VOLATILITY_PRICE;
            case static_cast<char>(117): return TRADE_BUST;
            case static_cast<char>(0): return NULL_VALUE;
        }

        throw std::runtime_error("unknown value for enum MDEntryType [E103]");
    }

    static const char *c_str(const MDEntryType::Value value)
    {
        switch (value)
        {
            case BID: return "BID";
            case OFFER: return "OFFER";
            case TRADE: return "TRADE";
            case INDEX_VALUE: return "INDEX_VALUE";
            case OPENING_PRICE: return "OPENING_PRICE";
            case CLOSING_PRICE: return "CLOSING_PRICE";
            case SETTLEMENT_PRICE: return "SETTLEMENT_PRICE";
            case SESSION_HIGH_PRICE: return "SESSION_HIGH_PRICE";
            case SESSION_LOW_PRICE: return "SESSION_LOW_PRICE";
            case EXECUTION_STATISTICS: return "EXECUTION_STATISTICS";
            case IMBALANCE: return "IMBALANCE";
            case TRADE_VOLUME: return "TRADE_VOLUME";
            case OPEN_INTEREST: return "OPEN_INTEREST";
            case EMPTY_BOOK: return "EMPTY_BOOK";
            case SECURITY_TRADING_STATE_PHASE: return "SECURITY_TRADING_STATE_PHASE";
            case PRICE_BAND: return "PRICE_BAND";
            case QUANTITY_BAND: return "QUANTITY_BAND";
            case COMPOSITE_UNDERLYING_PRICE: return "COMPOSITE_UNDERLYING_PRICE";
            case EXECUTION_SUMMARY: return "EXECUTION_SUMMARY";
            case VOLATILITY_PRICE: return "VOLATILITY_PRICE";
            case TRADE_BUST: return "TRADE_BUST";
            case NULL_VALUE: return "NULL_VALUE";
        }

        throw std::runtime_error("unknown value for enum MDEntryType [E103]:");
    }

    template<typename CharT, typename Traits>
    friend std::basic_ostream<CharT, Traits> & operator << (
        std::basic_ostream<CharT, Traits> &os, MDEntryType::Value m)
    {
        return os << MDEntryType::c_str(m);
    }
};

}
}
}
}

#endif
