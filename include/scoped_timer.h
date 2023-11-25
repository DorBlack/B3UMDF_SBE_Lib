//
// Created by santana on 11/18/23.
//

#ifndef UMDF_B3_SBE_SCOPED_TIMER_H
#define UMDF_B3_SBE_SCOPED_TIMER_H

#include <chrono>

namespace utils::profiling
{
    struct time_point {
        std::uint64_t  _M_ns_latency;
    };

    class latency_report {

    };

    class scoped_timer {
    public:
        using ClockType = std::chrono::steady_clock;

        scoped_timer(const char* __func):
        _M_func_name(__func), _M_start(ClockType::now())
        {
        }

        ~scoped_timer()
        {
            using namespace std::chrono;
            auto stop = ClockType::now();
            auto duration = (stop - _M_start);
            auto ns = duration_cast<nanoseconds>(duration).count();
        }
    private:
        const char* _M_func_name{};
        const ClockType::time_point _M_start{};
    };
}
#endif //UMDF_B3_SBE_SCOPED_TIMER_H
