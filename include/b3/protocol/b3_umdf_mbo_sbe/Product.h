/* Generated SBE (Simple Binary Encoding) message codec */
#ifndef _B3_UMDF_MBO_SBE_PRODUCT_CXX_H_
#define _B3_UMDF_MBO_SBE_PRODUCT_CXX_H_

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

class Product
{
public:
    enum Value
    {
        COMMODITY = static_cast<std::uint8_t>(2),
        CORPORATE = static_cast<std::uint8_t>(3),
        CURRENCY = static_cast<std::uint8_t>(4),
        EQUITY = static_cast<std::uint8_t>(5),
        GOVERNMENT = static_cast<std::uint8_t>(6),
        INDEX = static_cast<std::uint8_t>(7),
        ECONOMIC_INDICATOR = static_cast<std::uint8_t>(15),
        MULTILEG = static_cast<std::uint8_t>(16),
        NULL_VALUE = static_cast<std::uint8_t>(255)
    };

    static Product::Value get(const std::uint8_t value)
    {
        switch (value)
        {
            case static_cast<std::uint8_t>(2): return COMMODITY;
            case static_cast<std::uint8_t>(3): return CORPORATE;
            case static_cast<std::uint8_t>(4): return CURRENCY;
            case static_cast<std::uint8_t>(5): return EQUITY;
            case static_cast<std::uint8_t>(6): return GOVERNMENT;
            case static_cast<std::uint8_t>(7): return INDEX;
            case static_cast<std::uint8_t>(15): return ECONOMIC_INDICATOR;
            case static_cast<std::uint8_t>(16): return MULTILEG;
            case static_cast<std::uint8_t>(255): return NULL_VALUE;
        }

        throw std::runtime_error("unknown value for enum Product [E103]");
    }

    static const char *c_str(const Product::Value value)
    {
        switch (value)
        {
            case COMMODITY: return "COMMODITY";
            case CORPORATE: return "CORPORATE";
            case CURRENCY: return "CURRENCY";
            case EQUITY: return "EQUITY";
            case GOVERNMENT: return "GOVERNMENT";
            case INDEX: return "INDEX";
            case ECONOMIC_INDICATOR: return "ECONOMIC_INDICATOR";
            case MULTILEG: return "MULTILEG";
            case NULL_VALUE: return "NULL_VALUE";
        }

        throw std::runtime_error("unknown value for enum Product [E103]:");
    }

    template<typename CharT, typename Traits>
    friend std::basic_ostream<CharT, Traits> & operator << (
        std::basic_ostream<CharT, Traits> &os, Product::Value m)
    {
        return os << Product::c_str(m);
    }
};

}
}
}
}

#endif
