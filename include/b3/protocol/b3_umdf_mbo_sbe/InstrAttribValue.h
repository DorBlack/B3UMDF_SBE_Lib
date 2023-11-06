/* Generated SBE (Simple Binary Encoding) message codec */
#ifndef _B3_UMDF_MBO_SBE_INSTRATTRIBVALUE_CXX_H_
#define _B3_UMDF_MBO_SBE_INSTRATTRIBVALUE_CXX_H_

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

class InstrAttribValue
{
public:
    enum Value
    {
        ELECTRONIC_MATCH_OR_GTD_GTC_ELIGIBLE = static_cast<std::uint8_t>(1),
        ORDER_CROSS_ELIGIBLE = static_cast<std::uint8_t>(2),
        BLOCK_TRADE_ELIGIBLE = static_cast<std::uint8_t>(3),
        FLAG_RFQ_FOR_CROSS_ELIGIBLE = static_cast<std::uint8_t>(14),
        NEGOTIATED_QUOTE_ELIGIBLE = static_cast<std::uint8_t>(17),
        NULL_VALUE = static_cast<std::uint8_t>(255)
    };

    static InstrAttribValue::Value get(const std::uint8_t value)
    {
        switch (value)
        {
            case static_cast<std::uint8_t>(1): return ELECTRONIC_MATCH_OR_GTD_GTC_ELIGIBLE;
            case static_cast<std::uint8_t>(2): return ORDER_CROSS_ELIGIBLE;
            case static_cast<std::uint8_t>(3): return BLOCK_TRADE_ELIGIBLE;
            case static_cast<std::uint8_t>(14): return FLAG_RFQ_FOR_CROSS_ELIGIBLE;
            case static_cast<std::uint8_t>(17): return NEGOTIATED_QUOTE_ELIGIBLE;
            case static_cast<std::uint8_t>(255): return NULL_VALUE;
        }

        throw std::runtime_error("unknown value for enum InstrAttribValue [E103]");
    }

    static const char *c_str(const InstrAttribValue::Value value)
    {
        switch (value)
        {
            case ELECTRONIC_MATCH_OR_GTD_GTC_ELIGIBLE: return "ELECTRONIC_MATCH_OR_GTD_GTC_ELIGIBLE";
            case ORDER_CROSS_ELIGIBLE: return "ORDER_CROSS_ELIGIBLE";
            case BLOCK_TRADE_ELIGIBLE: return "BLOCK_TRADE_ELIGIBLE";
            case FLAG_RFQ_FOR_CROSS_ELIGIBLE: return "FLAG_RFQ_FOR_CROSS_ELIGIBLE";
            case NEGOTIATED_QUOTE_ELIGIBLE: return "NEGOTIATED_QUOTE_ELIGIBLE";
            case NULL_VALUE: return "NULL_VALUE";
        }

        throw std::runtime_error("unknown value for enum InstrAttribValue [E103]:");
    }

    template<typename CharT, typename Traits>
    friend std::basic_ostream<CharT, Traits> & operator << (
        std::basic_ostream<CharT, Traits> &os, InstrAttribValue::Value m)
    {
        return os << InstrAttribValue::c_str(m);
    }
};

}
}
}
}

#endif
