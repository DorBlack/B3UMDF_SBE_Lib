/* Generated SBE (Simple Binary Encoding) message codec */
#ifndef _B3_UMDF_MBO_SBE_MULTILEGPRICEMETHOD_CXX_H_
#define _B3_UMDF_MBO_SBE_MULTILEGPRICEMETHOD_CXX_H_

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

class MultiLegPriceMethod
{
public:
    enum Value
    {
        NET_PRICE = static_cast<std::uint8_t>(0),
        REVERSED_NET_PRICE = static_cast<std::uint8_t>(1),
        YIELD_DIFFERENCE = static_cast<std::uint8_t>(2),
        INDIVIDUAL = static_cast<std::uint8_t>(3),
        CONTRACT_WEIGHTED_AVERAGE_PRICE = static_cast<std::uint8_t>(4),
        MULTIPLIED_PRICE = static_cast<std::uint8_t>(5),
        NULL_VALUE = static_cast<std::uint8_t>(255)
    };

    static MultiLegPriceMethod::Value get(const std::uint8_t value)
    {
        switch (value)
        {
            case static_cast<std::uint8_t>(0): return NET_PRICE;
            case static_cast<std::uint8_t>(1): return REVERSED_NET_PRICE;
            case static_cast<std::uint8_t>(2): return YIELD_DIFFERENCE;
            case static_cast<std::uint8_t>(3): return INDIVIDUAL;
            case static_cast<std::uint8_t>(4): return CONTRACT_WEIGHTED_AVERAGE_PRICE;
            case static_cast<std::uint8_t>(5): return MULTIPLIED_PRICE;
            case static_cast<std::uint8_t>(255): return NULL_VALUE;
        }

        throw std::runtime_error("unknown value for enum MultiLegPriceMethod [E103]");
    }

    static const char *c_str(const MultiLegPriceMethod::Value value)
    {
        switch (value)
        {
            case NET_PRICE: return "NET_PRICE";
            case REVERSED_NET_PRICE: return "REVERSED_NET_PRICE";
            case YIELD_DIFFERENCE: return "YIELD_DIFFERENCE";
            case INDIVIDUAL: return "INDIVIDUAL";
            case CONTRACT_WEIGHTED_AVERAGE_PRICE: return "CONTRACT_WEIGHTED_AVERAGE_PRICE";
            case MULTIPLIED_PRICE: return "MULTIPLIED_PRICE";
            case NULL_VALUE: return "NULL_VALUE";
        }

        throw std::runtime_error("unknown value for enum MultiLegPriceMethod [E103]:");
    }

    template<typename CharT, typename Traits>
    friend std::basic_ostream<CharT, Traits> & operator << (
        std::basic_ostream<CharT, Traits> &os, MultiLegPriceMethod::Value m)
    {
        return os << MultiLegPriceMethod::c_str(m);
    }
};

}
}
}
}

#endif
