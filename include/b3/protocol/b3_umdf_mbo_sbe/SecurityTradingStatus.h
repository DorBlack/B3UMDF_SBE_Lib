/* Generated SBE (Simple Binary Encoding) message codec */
#ifndef _B3_UMDF_MBO_SBE_SECURITYTRADINGSTATUS_CXX_H_
#define _B3_UMDF_MBO_SBE_SECURITYTRADINGSTATUS_CXX_H_

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

class SecurityTradingStatus
{
public:
    enum Value
    {
        PAUSE = static_cast<std::uint8_t>(2),
        CLOSE = static_cast<std::uint8_t>(4),
        OPEN = static_cast<std::uint8_t>(17),
        FORBIDDEN = static_cast<std::uint8_t>(18),
        UNKNOWN_OR_INVALID = static_cast<std::uint8_t>(20),
        RESERVED = static_cast<std::uint8_t>(21),
        FINAL_CLOSING_CALL = static_cast<std::uint8_t>(101),
        NULL_VALUE = static_cast<std::uint8_t>(255)
    };

    static SecurityTradingStatus::Value get(const std::uint8_t value)
    {
        switch (value)
        {
            case static_cast<std::uint8_t>(2): return PAUSE;
            case static_cast<std::uint8_t>(4): return CLOSE;
            case static_cast<std::uint8_t>(17): return OPEN;
            case static_cast<std::uint8_t>(18): return FORBIDDEN;
            case static_cast<std::uint8_t>(20): return UNKNOWN_OR_INVALID;
            case static_cast<std::uint8_t>(21): return RESERVED;
            case static_cast<std::uint8_t>(101): return FINAL_CLOSING_CALL;
            case static_cast<std::uint8_t>(255): return NULL_VALUE;
        }

        throw std::runtime_error("unknown value for enum SecurityTradingStatus [E103]");
    }

    static const char *c_str(const SecurityTradingStatus::Value value)
    {
        switch (value)
        {
            case PAUSE: return "PAUSE";
            case CLOSE: return "CLOSE";
            case OPEN: return "OPEN";
            case FORBIDDEN: return "FORBIDDEN";
            case UNKNOWN_OR_INVALID: return "UNKNOWN_OR_INVALID";
            case RESERVED: return "RESERVED";
            case FINAL_CLOSING_CALL: return "FINAL_CLOSING_CALL";
            case NULL_VALUE: return "NULL_VALUE";
        }

        throw std::runtime_error("unknown value for enum SecurityTradingStatus [E103]:");
    }

    template<typename CharT, typename Traits>
    friend std::basic_ostream<CharT, Traits> & operator << (
        std::basic_ostream<CharT, Traits> &os, SecurityTradingStatus::Value m)
    {
        return os << SecurityTradingStatus::c_str(m);
    }
};

}
}
}
}

#endif
