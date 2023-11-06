/* Generated SBE (Simple Binary Encoding) message codec */
#ifndef _B3_UMDF_MBO_SBE_NEWSSOURCE_CXX_H_
#define _B3_UMDF_MBO_SBE_NEWSSOURCE_CXX_H_

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

class NewsSource
{
public:
    enum Value
    {
        OTHER = static_cast<std::uint8_t>(0),
        DCM = static_cast<std::uint8_t>(1),
        BBMNET = static_cast<std::uint8_t>(2),
        MARKET_SURVEILLANCE = static_cast<std::uint8_t>(3),
        INTERNET = static_cast<std::uint8_t>(4),
        DPR_VE = static_cast<std::uint8_t>(5),
        MKT_OPS_FX_AGENCY = static_cast<std::uint8_t>(19),
        MKT_OPS_DERIVATIVES_AGENCY = static_cast<std::uint8_t>(20),
        OVER_THE_COUNTER_NEWS_AGENCY = static_cast<std::uint8_t>(11),
        ELECTRONIC_PURCHASE_EXCHANGE = static_cast<std::uint8_t>(13),
        CBLC_NEWS_AGENCY = static_cast<std::uint8_t>(14),
        BOVESPA_INDEX_AGENCY = static_cast<std::uint8_t>(15),
        BOVESPA_INSTITUTIONAL_AGENCY = static_cast<std::uint8_t>(16),
        MKT_OPS_EQUITIES_AGENCY = static_cast<std::uint8_t>(17),
        BOVESPA_COMPANIES_AGENCY = static_cast<std::uint8_t>(18),
        NULL_VALUE = static_cast<std::uint8_t>(255)
    };

    static NewsSource::Value get(const std::uint8_t value)
    {
        switch (value)
        {
            case static_cast<std::uint8_t>(0): return OTHER;
            case static_cast<std::uint8_t>(1): return DCM;
            case static_cast<std::uint8_t>(2): return BBMNET;
            case static_cast<std::uint8_t>(3): return MARKET_SURVEILLANCE;
            case static_cast<std::uint8_t>(4): return INTERNET;
            case static_cast<std::uint8_t>(5): return DPR_VE;
            case static_cast<std::uint8_t>(19): return MKT_OPS_FX_AGENCY;
            case static_cast<std::uint8_t>(20): return MKT_OPS_DERIVATIVES_AGENCY;
            case static_cast<std::uint8_t>(11): return OVER_THE_COUNTER_NEWS_AGENCY;
            case static_cast<std::uint8_t>(13): return ELECTRONIC_PURCHASE_EXCHANGE;
            case static_cast<std::uint8_t>(14): return CBLC_NEWS_AGENCY;
            case static_cast<std::uint8_t>(15): return BOVESPA_INDEX_AGENCY;
            case static_cast<std::uint8_t>(16): return BOVESPA_INSTITUTIONAL_AGENCY;
            case static_cast<std::uint8_t>(17): return MKT_OPS_EQUITIES_AGENCY;
            case static_cast<std::uint8_t>(18): return BOVESPA_COMPANIES_AGENCY;
            case static_cast<std::uint8_t>(255): return NULL_VALUE;
        }

        throw std::runtime_error("unknown value for enum NewsSource [E103]");
    }

    static const char *c_str(const NewsSource::Value value)
    {
        switch (value)
        {
            case OTHER: return "OTHER";
            case DCM: return "DCM";
            case BBMNET: return "BBMNET";
            case MARKET_SURVEILLANCE: return "MARKET_SURVEILLANCE";
            case INTERNET: return "INTERNET";
            case DPR_VE: return "DPR_VE";
            case MKT_OPS_FX_AGENCY: return "MKT_OPS_FX_AGENCY";
            case MKT_OPS_DERIVATIVES_AGENCY: return "MKT_OPS_DERIVATIVES_AGENCY";
            case OVER_THE_COUNTER_NEWS_AGENCY: return "OVER_THE_COUNTER_NEWS_AGENCY";
            case ELECTRONIC_PURCHASE_EXCHANGE: return "ELECTRONIC_PURCHASE_EXCHANGE";
            case CBLC_NEWS_AGENCY: return "CBLC_NEWS_AGENCY";
            case BOVESPA_INDEX_AGENCY: return "BOVESPA_INDEX_AGENCY";
            case BOVESPA_INSTITUTIONAL_AGENCY: return "BOVESPA_INSTITUTIONAL_AGENCY";
            case MKT_OPS_EQUITIES_AGENCY: return "MKT_OPS_EQUITIES_AGENCY";
            case BOVESPA_COMPANIES_AGENCY: return "BOVESPA_COMPANIES_AGENCY";
            case NULL_VALUE: return "NULL_VALUE";
        }

        throw std::runtime_error("unknown value for enum NewsSource [E103]:");
    }

    template<typename CharT, typename Traits>
    friend std::basic_ostream<CharT, Traits> & operator << (
        std::basic_ostream<CharT, Traits> &os, NewsSource::Value m)
    {
        return os << NewsSource::c_str(m);
    }
};

}
}
}
}

#endif
