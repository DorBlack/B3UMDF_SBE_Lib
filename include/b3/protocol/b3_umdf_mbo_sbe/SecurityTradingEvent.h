/* Generated SBE (Simple Binary Encoding) message codec */
#ifndef _B3_UMDF_MBO_SBE_SECURITYTRADINGEVENT_CXX_H_
#define _B3_UMDF_MBO_SBE_SECURITYTRADINGEVENT_CXX_H_

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

class SecurityTradingEvent
{
public:
    enum Value
    {
        TRADING_SESSION_CHANGE = static_cast<std::uint8_t>(4),
        SECURITY_STATUS_CHANGE = static_cast<std::uint8_t>(101),
        SECURITY_REJOINS_SECURITY_GROUP_STATUS = static_cast<std::uint8_t>(102),
        NULL_VALUE = static_cast<std::uint8_t>(255)
    };

    static SecurityTradingEvent::Value get(const std::uint8_t value)
    {
        switch (value)
        {
            case static_cast<std::uint8_t>(4): return TRADING_SESSION_CHANGE;
            case static_cast<std::uint8_t>(101): return SECURITY_STATUS_CHANGE;
            case static_cast<std::uint8_t>(102): return SECURITY_REJOINS_SECURITY_GROUP_STATUS;
            case static_cast<std::uint8_t>(255): return NULL_VALUE;
        }

        throw std::runtime_error("unknown value for enum SecurityTradingEvent [E103]");
    }

    static const char *c_str(const SecurityTradingEvent::Value value)
    {
        switch (value)
        {
            case TRADING_SESSION_CHANGE: return "TRADING_SESSION_CHANGE";
            case SECURITY_STATUS_CHANGE: return "SECURITY_STATUS_CHANGE";
            case SECURITY_REJOINS_SECURITY_GROUP_STATUS: return "SECURITY_REJOINS_SECURITY_GROUP_STATUS";
            case NULL_VALUE: return "NULL_VALUE";
        }

        throw std::runtime_error("unknown value for enum SecurityTradingEvent [E103]:");
    }

    template<typename CharT, typename Traits>
    friend std::basic_ostream<CharT, Traits> & operator << (
        std::basic_ostream<CharT, Traits> &os, SecurityTradingEvent::Value m)
    {
        return os << SecurityTradingEvent::c_str(m);
    }
};

}
}
}
}

#endif
