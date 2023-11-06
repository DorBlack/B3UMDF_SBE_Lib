/* Generated SBE (Simple Binary Encoding) message codec */
#ifndef _B3_UMDF_MBO_SBE_MESSAGETYPE_CXX_H_
#define _B3_UMDF_MBO_SBE_MESSAGETYPE_CXX_H_

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

class MessageType
{
public:
    enum Value
    {
        Sequence = static_cast<char>(48),
        SequenceReset = static_cast<char>(52),
        MarketDataIncrementalRefresh = static_cast<char>(88),
        SecurityStatus = static_cast<char>(102),
        SecurityDefinition = static_cast<char>(100),
        News = static_cast<char>(66),
        MarketDataSnapshotFullRefresh = static_cast<char>(87),
        NULL_VALUE = static_cast<char>(0)
    };

    static MessageType::Value get(const char value)
    {
        switch (value)
        {
            case static_cast<char>(48): return Sequence;
            case static_cast<char>(52): return SequenceReset;
            case static_cast<char>(88): return MarketDataIncrementalRefresh;
            case static_cast<char>(102): return SecurityStatus;
            case static_cast<char>(100): return SecurityDefinition;
            case static_cast<char>(66): return News;
            case static_cast<char>(87): return MarketDataSnapshotFullRefresh;
            case static_cast<char>(0): return NULL_VALUE;
        }

        throw std::runtime_error("unknown value for enum MessageType [E103]");
    }

    static const char *c_str(const MessageType::Value value)
    {
        switch (value)
        {
            case Sequence: return "Sequence";
            case SequenceReset: return "SequenceReset";
            case MarketDataIncrementalRefresh: return "MarketDataIncrementalRefresh";
            case SecurityStatus: return "SecurityStatus";
            case SecurityDefinition: return "SecurityDefinition";
            case News: return "News";
            case MarketDataSnapshotFullRefresh: return "MarketDataSnapshotFullRefresh";
            case NULL_VALUE: return "NULL_VALUE";
        }

        throw std::runtime_error("unknown value for enum MessageType [E103]:");
    }

    template<typename CharT, typename Traits>
    friend std::basic_ostream<CharT, Traits> & operator << (
        std::basic_ostream<CharT, Traits> &os, MessageType::Value m)
    {
        return os << MessageType::c_str(m);
    }
};

}
}
}
}

#endif
