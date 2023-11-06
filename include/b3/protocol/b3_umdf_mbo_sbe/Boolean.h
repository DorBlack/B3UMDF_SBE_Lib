/* Generated SBE (Simple Binary Encoding) message codec */
#ifndef _B3_UMDF_MBO_SBE_BOOLEAN_CXX_H_
#define _B3_UMDF_MBO_SBE_BOOLEAN_CXX_H_

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

class Boolean
{
public:
    enum Value
    {
        FALSE_VALUE = static_cast<std::uint8_t>(0),
        TRUE_VALUE = static_cast<std::uint8_t>(1),
        NULL_VALUE = static_cast<std::uint8_t>(255)
    };

    static Boolean::Value get(const std::uint8_t value)
    {
        switch (value)
        {
            case static_cast<std::uint8_t>(0): return FALSE_VALUE;
            case static_cast<std::uint8_t>(1): return TRUE_VALUE;
            case static_cast<std::uint8_t>(255): return NULL_VALUE;
        }

        throw std::runtime_error("unknown value for enum Boolean [E103]");
    }

    static const char *c_str(const Boolean::Value value)
    {
        switch (value)
        {
            case FALSE_VALUE: return "FALSE_VALUE";
            case TRUE_VALUE: return "TRUE_VALUE";
            case NULL_VALUE: return "NULL_VALUE";
        }

        throw std::runtime_error("unknown value for enum Boolean [E103]:");
    }

    template<typename CharT, typename Traits>
    friend std::basic_ostream<CharT, Traits> & operator << (
        std::basic_ostream<CharT, Traits> &os, Boolean::Value m)
    {
        return os << Boolean::c_str(m);
    }
};

}
}
}
}

#endif
