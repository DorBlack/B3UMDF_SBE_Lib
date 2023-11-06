/* Generated SBE (Simple Binary Encoding) message codec */
#ifndef _B3_UMDF_MBO_SBE_SECURITYIDSOURCE_CXX_H_
#define _B3_UMDF_MBO_SBE_SECURITYIDSOURCE_CXX_H_

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

class SecurityIDSource
{
public:
    enum Value
    {
        ISIN = static_cast<char>(52),
        EXCHANGE_SYMBOL = static_cast<char>(56),
        NULL_VALUE = static_cast<char>(0)
    };

    static SecurityIDSource::Value get(const char value)
    {
        switch (value)
        {
            case static_cast<char>(52): return ISIN;
            case static_cast<char>(56): return EXCHANGE_SYMBOL;
            case static_cast<char>(0): return NULL_VALUE;
        }

        throw std::runtime_error("unknown value for enum SecurityIDSource [E103]");
    }

    static const char *c_str(const SecurityIDSource::Value value)
    {
        switch (value)
        {
            case ISIN: return "ISIN";
            case EXCHANGE_SYMBOL: return "EXCHANGE_SYMBOL";
            case NULL_VALUE: return "NULL_VALUE";
        }

        throw std::runtime_error("unknown value for enum SecurityIDSource [E103]:");
    }

    template<typename CharT, typename Traits>
    friend std::basic_ostream<CharT, Traits> & operator << (
        std::basic_ostream<CharT, Traits> &os, SecurityIDSource::Value m)
    {
        return os << SecurityIDSource::c_str(m);
    }
};

}
}
}
}

#endif
