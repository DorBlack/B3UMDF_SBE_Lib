/* Generated SBE (Simple Binary Encoding) message codec */
#ifndef _B3_UMDF_MBO_SBE_TRDSUBTYPE_CXX_H_
#define _B3_UMDF_MBO_SBE_TRDSUBTYPE_CXX_H_

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

class TrdSubType
{
public:
    enum Value
    {
        MULTI_ASSET_TRADE = static_cast<std::uint8_t>(101),
        LEG_TRADE = static_cast<std::uint8_t>(102),
        MIDPOINT_TRADE = static_cast<std::uint8_t>(103),
        BLOCK_BOOK_TRADE = static_cast<std::uint8_t>(104),
        RFQ_TRADE = static_cast<std::uint8_t>(105),
        RLP_TRADE = static_cast<std::uint8_t>(106),
        TAC_TRADE = static_cast<std::uint8_t>(107),
        TAA_TRADE = static_cast<std::uint8_t>(108),
        NULL_VALUE = static_cast<std::uint8_t>(0)
    };

    static TrdSubType::Value get(const std::uint8_t value)
    {
        switch (value)
        {
            case static_cast<std::uint8_t>(101): return MULTI_ASSET_TRADE;
            case static_cast<std::uint8_t>(102): return LEG_TRADE;
            case static_cast<std::uint8_t>(103): return MIDPOINT_TRADE;
            case static_cast<std::uint8_t>(104): return BLOCK_BOOK_TRADE;
            case static_cast<std::uint8_t>(105): return RFQ_TRADE;
            case static_cast<std::uint8_t>(106): return RLP_TRADE;
            case static_cast<std::uint8_t>(107): return TAC_TRADE;
            case static_cast<std::uint8_t>(108): return TAA_TRADE;
            case static_cast<std::uint8_t>(0): return NULL_VALUE;
        }

        throw std::runtime_error("unknown value for enum TrdSubType [E103]");
    }

    static const char *c_str(const TrdSubType::Value value)
    {
        switch (value)
        {
            case MULTI_ASSET_TRADE: return "MULTI_ASSET_TRADE";
            case LEG_TRADE: return "LEG_TRADE";
            case MIDPOINT_TRADE: return "MIDPOINT_TRADE";
            case BLOCK_BOOK_TRADE: return "BLOCK_BOOK_TRADE";
            case RFQ_TRADE: return "RFQ_TRADE";
            case RLP_TRADE: return "RLP_TRADE";
            case TAC_TRADE: return "TAC_TRADE";
            case TAA_TRADE: return "TAA_TRADE";
            case NULL_VALUE: return "NULL_VALUE";
        }

        throw std::runtime_error("unknown value for enum TrdSubType [E103]:");
    }

    template<typename CharT, typename Traits>
    friend std::basic_ostream<CharT, Traits> & operator << (
        std::basic_ostream<CharT, Traits> &os, TrdSubType::Value m)
    {
        return os << TrdSubType::c_str(m);
    }
};

}
}
}
}

#endif
