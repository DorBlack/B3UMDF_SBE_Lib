// Copyright (c)  2023-${current_year},  Fábio da Silva Santana
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
//

//
// Created by scien on 06/11/2023.
//

#include "interface/b3_umdf_sbe_api.hpp"
#include <iostream>
#include "time.h"

int main(int argc, char** argv)
{
    struct timespec _timer;

    constexpr size_t tot = 300000;
    long results[tot] = {0x00};
    size_t current = 0;

    auto output = std::make_shared<b3::umdf::sbe::channel_notification>();
    output->on_security_def = [&](auto msg) {
        std::cout << "new msg" << std::endl;
    };
    output->on_incremental = [&](auto msg) {
        clock_gettime(CLOCK_MONOTONIC, &_timer);
        long result = 0;
        if(current == tot)
        {
            for(size_t x = 0, w = 0; x < tot; ++w)
            {
                for(size_t y = 0; y < 100; ++y, ++x)
                {
                    result += results[x];
                }
                std::cout << w << ": " << result / 100 << " ns\n ";
                result = 0;
            }
            int i;
            std::cin >> i;
        }
        results[current++] = _timer.tv_nsec - msg->get_created_time_nano();
    };
    output->on_snapshot = [&](auto msg) {

    };

    auto config = b3::engine::channel_config();

    config.instrument_def.address = "/data/072_mbo_security_definition.pcap";
    config.snapshot.address = "/data/072_mbo_snapshot_recovery.pcap";
    config.feed_a.address = "/data/072_mbo_incremental_feedA.pcap";

    auto channel = b3::umdf::sbe::backtest_channel(config, output);
    channel.start();

    std::cout << "press any key to stop" << std::endl;
    int i = 0;
    std::cin >> i;
    return 0;
}
