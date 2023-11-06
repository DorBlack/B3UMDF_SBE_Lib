/* Generated SBE (Simple Binary Encoding) message codec */
#ifndef _B3_UMDF_MBO_SBE_GOVERNANCEINDICATOR_CXX_H_
#define _B3_UMDF_MBO_SBE_GOVERNANCEINDICATOR_CXX_H_

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

class GovernanceIndicator
{
public:
    enum Value
    {
        No = static_cast<std::uint8_t>(0),
        N1 = static_cast<std::uint8_t>(1),
        N2 = static_cast<std::uint8_t>(2),
        NM = static_cast<std::uint8_t>(4),
        MA = static_cast<std::uint8_t>(5),
        MB = static_cast<std::uint8_t>(6),
        M2 = static_cast<std::uint8_t>(7),
        NULL_VALUE = static_cast<std::uint8_t>(255)
    };

    static GovernanceIndicator::Value get(const std::uint8_t value)
    {
        switch (value)
        {
            case static_cast<std::uint8_t>(0): return No;
            case static_cast<std::uint8_t>(1): return N1;
            case static_cast<std::uint8_t>(2): return N2;
            case static_cast<std::uint8_t>(4): return NM;
            case static_cast<std::uint8_t>(5): return MA;
            case static_cast<std::uint8_t>(6): return MB;
            case static_cast<std::uint8_t>(7): return M2;
            case static_cast<std::uint8_t>(255): return NULL_VALUE;
        }

        throw std::runtime_error("unknown value for enum GovernanceIndicator [E103]");
    }

    static const char *c_str(const GovernanceIndicator::Value value)
    {
        switch (value)
        {
            case No: return "No";
            case N1: return "N1";
            case N2: return "N2";
            case NM: return "NM";
            case MA: return "MA";
            case MB: return "MB";
            case M2: return "M2";
            case NULL_VALUE: return "NULL_VALUE";
        }

        throw std::runtime_error("unknown value for enum GovernanceIndicator [E103]:");
    }

    template<typename CharT, typename Traits>
    friend std::basic_ostream<CharT, Traits> & operator << (
        std::basic_ostream<CharT, Traits> &os, GovernanceIndicator::Value m)
    {
        return os << GovernanceIndicator::c_str(m);
    }
};

}
}
}
}

#endif
