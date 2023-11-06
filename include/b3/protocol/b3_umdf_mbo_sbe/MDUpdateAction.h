/* Generated SBE (Simple Binary Encoding) message codec */
#ifndef _B3_UMDF_MBO_SBE_MDUPDATEACTION_CXX_H_
#define _B3_UMDF_MBO_SBE_MDUPDATEACTION_CXX_H_

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

class MDUpdateAction
{
public:
    enum Value
    {
        NEW = static_cast<std::uint8_t>(0),
        CHANGE = static_cast<std::uint8_t>(1),
        DELETE = static_cast<std::uint8_t>(2),
        DELETE_THRU = static_cast<std::uint8_t>(3),
        DELETE_FROM = static_cast<std::uint8_t>(4),
        OVERLAY = static_cast<std::uint8_t>(5),
        NULL_VALUE = static_cast<std::uint8_t>(255)
    };

    static MDUpdateAction::Value get(const std::uint8_t value)
    {
        switch (value)
        {
            case static_cast<std::uint8_t>(0): return NEW;
            case static_cast<std::uint8_t>(1): return CHANGE;
            case static_cast<std::uint8_t>(2): return DELETE;
            case static_cast<std::uint8_t>(3): return DELETE_THRU;
            case static_cast<std::uint8_t>(4): return DELETE_FROM;
            case static_cast<std::uint8_t>(5): return OVERLAY;
            case static_cast<std::uint8_t>(255): return NULL_VALUE;
        }

        throw std::runtime_error("unknown value for enum MDUpdateAction [E103]");
    }

    static const char *c_str(const MDUpdateAction::Value value)
    {
        switch (value)
        {
            case NEW: return "NEW";
            case CHANGE: return "CHANGE";
            case DELETE: return "DELETE";
            case DELETE_THRU: return "DELETE_THRU";
            case DELETE_FROM: return "DELETE_FROM";
            case OVERLAY: return "OVERLAY";
            case NULL_VALUE: return "NULL_VALUE";
        }

        throw std::runtime_error("unknown value for enum MDUpdateAction [E103]:");
    }

    template<typename CharT, typename Traits>
    friend std::basic_ostream<CharT, Traits> & operator << (
        std::basic_ostream<CharT, Traits> &os, MDUpdateAction::Value m)
    {
        return os << MDUpdateAction::c_str(m);
    }
};

}
}
}
}

#endif
