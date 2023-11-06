/* Generated SBE (Simple Binary Encoding) message codec */
#ifndef _B3_UMDF_MBO_SBE_OPENCLOSESETTLFLAG_CXX_H_
#define _B3_UMDF_MBO_SBE_OPENCLOSESETTLFLAG_CXX_H_

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

class OpenCloseSettlFlag
{
public:
    enum Value
    {
        DAILY = static_cast<std::uint8_t>(0),
        SESSION = static_cast<std::uint8_t>(1),
        EXPECTED_ENTRY = static_cast<std::uint8_t>(3),
        ENTRY_FROM_PREVIOUS_BUSINESS_DAY = static_cast<std::uint8_t>(4),
        THEORETICAL_PRICE = static_cast<std::uint8_t>(5),
        NULL_VALUE = static_cast<std::uint8_t>(255)
    };

    static OpenCloseSettlFlag::Value get(const std::uint8_t value)
    {
        switch (value)
        {
            case static_cast<std::uint8_t>(0): return DAILY;
            case static_cast<std::uint8_t>(1): return SESSION;
            case static_cast<std::uint8_t>(3): return EXPECTED_ENTRY;
            case static_cast<std::uint8_t>(4): return ENTRY_FROM_PREVIOUS_BUSINESS_DAY;
            case static_cast<std::uint8_t>(5): return THEORETICAL_PRICE;
            case static_cast<std::uint8_t>(255): return NULL_VALUE;
        }

        throw std::runtime_error("unknown value for enum OpenCloseSettlFlag [E103]");
    }

    static const char *c_str(const OpenCloseSettlFlag::Value value)
    {
        switch (value)
        {
            case DAILY: return "DAILY";
            case SESSION: return "SESSION";
            case EXPECTED_ENTRY: return "EXPECTED_ENTRY";
            case ENTRY_FROM_PREVIOUS_BUSINESS_DAY: return "ENTRY_FROM_PREVIOUS_BUSINESS_DAY";
            case THEORETICAL_PRICE: return "THEORETICAL_PRICE";
            case NULL_VALUE: return "NULL_VALUE";
        }

        throw std::runtime_error("unknown value for enum OpenCloseSettlFlag [E103]:");
    }

    template<typename CharT, typename Traits>
    friend std::basic_ostream<CharT, Traits> & operator << (
        std::basic_ostream<CharT, Traits> &os, OpenCloseSettlFlag::Value m)
    {
        return os << OpenCloseSettlFlag::c_str(m);
    }
};

}
}
}
}

#endif
