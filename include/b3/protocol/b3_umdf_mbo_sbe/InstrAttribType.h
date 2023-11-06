/* Generated SBE (Simple Binary Encoding) message codec */
#ifndef _B3_UMDF_MBO_SBE_INSTRATTRIBTYPE_CXX_H_
#define _B3_UMDF_MBO_SBE_INSTRATTRIBTYPE_CXX_H_

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

class InstrAttribType
{
public:
    enum Value
    {
        TRADE_TYPE_ELIGIBILITY = static_cast<std::uint8_t>(24),
        GTD_GTC_ELIGIBILITY = static_cast<std::uint8_t>(34),
        NULL_VALUE = static_cast<std::uint8_t>(255)
    };

    static InstrAttribType::Value get(const std::uint8_t value)
    {
        switch (value)
        {
            case static_cast<std::uint8_t>(24): return TRADE_TYPE_ELIGIBILITY;
            case static_cast<std::uint8_t>(34): return GTD_GTC_ELIGIBILITY;
            case static_cast<std::uint8_t>(255): return NULL_VALUE;
        }

        throw std::runtime_error("unknown value for enum InstrAttribType [E103]");
    }

    static const char *c_str(const InstrAttribType::Value value)
    {
        switch (value)
        {
            case TRADE_TYPE_ELIGIBILITY: return "TRADE_TYPE_ELIGIBILITY";
            case GTD_GTC_ELIGIBILITY: return "GTD_GTC_ELIGIBILITY";
            case NULL_VALUE: return "NULL_VALUE";
        }

        throw std::runtime_error("unknown value for enum InstrAttribType [E103]:");
    }

    template<typename CharT, typename Traits>
    friend std::basic_ostream<CharT, Traits> & operator << (
        std::basic_ostream<CharT, Traits> &os, InstrAttribType::Value m)
    {
        return os << InstrAttribType::c_str(m);
    }
};

}
}
}
}

#endif
