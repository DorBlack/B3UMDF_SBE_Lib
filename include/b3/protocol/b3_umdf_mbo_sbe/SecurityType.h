/* Generated SBE (Simple Binary Encoding) message codec */
#ifndef _B3_UMDF_MBO_SBE_SECURITYTYPE_CXX_H_
#define _B3_UMDF_MBO_SBE_SECURITYTYPE_CXX_H_

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

class SecurityType
{
public:
    enum Value
    {
        CASH = static_cast<std::uint8_t>(1),
        CORP = static_cast<std::uint8_t>(2),
        CS = static_cast<std::uint8_t>(3),
        DTERM = static_cast<std::uint8_t>(4),
        ETF = static_cast<std::uint8_t>(5),
        FOPT = static_cast<std::uint8_t>(6),
        FORWARD = static_cast<std::uint8_t>(7),
        FUT = static_cast<std::uint8_t>(8),
        INDEX = static_cast<std::uint8_t>(9),
        INDEXOPT = static_cast<std::uint8_t>(10),
        MLEG = static_cast<std::uint8_t>(11),
        OPT = static_cast<std::uint8_t>(12),
        OPTEXER = static_cast<std::uint8_t>(13),
        PS = static_cast<std::uint8_t>(14),
        SECLOAN = static_cast<std::uint8_t>(15),
        SOPT = static_cast<std::uint8_t>(16),
        SPOT = static_cast<std::uint8_t>(17),
        NULL_VALUE = static_cast<std::uint8_t>(255)
    };

    static SecurityType::Value get(const std::uint8_t value)
    {
        switch (value)
        {
            case static_cast<std::uint8_t>(1): return CASH;
            case static_cast<std::uint8_t>(2): return CORP;
            case static_cast<std::uint8_t>(3): return CS;
            case static_cast<std::uint8_t>(4): return DTERM;
            case static_cast<std::uint8_t>(5): return ETF;
            case static_cast<std::uint8_t>(6): return FOPT;
            case static_cast<std::uint8_t>(7): return FORWARD;
            case static_cast<std::uint8_t>(8): return FUT;
            case static_cast<std::uint8_t>(9): return INDEX;
            case static_cast<std::uint8_t>(10): return INDEXOPT;
            case static_cast<std::uint8_t>(11): return MLEG;
            case static_cast<std::uint8_t>(12): return OPT;
            case static_cast<std::uint8_t>(13): return OPTEXER;
            case static_cast<std::uint8_t>(14): return PS;
            case static_cast<std::uint8_t>(15): return SECLOAN;
            case static_cast<std::uint8_t>(16): return SOPT;
            case static_cast<std::uint8_t>(17): return SPOT;
            case static_cast<std::uint8_t>(255): return NULL_VALUE;
        }

        throw std::runtime_error("unknown value for enum SecurityType [E103]");
    }

    static const char *c_str(const SecurityType::Value value)
    {
        switch (value)
        {
            case CASH: return "CASH";
            case CORP: return "CORP";
            case CS: return "CS";
            case DTERM: return "DTERM";
            case ETF: return "ETF";
            case FOPT: return "FOPT";
            case FORWARD: return "FORWARD";
            case FUT: return "FUT";
            case INDEX: return "INDEX";
            case INDEXOPT: return "INDEXOPT";
            case MLEG: return "MLEG";
            case OPT: return "OPT";
            case OPTEXER: return "OPTEXER";
            case PS: return "PS";
            case SECLOAN: return "SECLOAN";
            case SOPT: return "SOPT";
            case SPOT: return "SPOT";
            case NULL_VALUE: return "NULL_VALUE";
        }

        throw std::runtime_error("unknown value for enum SecurityType [E103]:");
    }

    template<typename CharT, typename Traits>
    friend std::basic_ostream<CharT, Traits> & operator << (
        std::basic_ostream<CharT, Traits> &os, SecurityType::Value m)
    {
        return os << SecurityType::c_str(m);
    }
};

}
}
}
}

#endif
