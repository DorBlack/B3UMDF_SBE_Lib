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

#include <iostream>
#include "b3/channel.hpp"
#include "time.h"
#include <chrono>

using namespace b3::protocol;
int main(int argc, char** argv)
{
    struct timespec _timer;
    int index = 0x00;
    double results[3000];
    auto output = std::make_shared<b3::umdf::channel::channel_notification>();
    output->on_security_def = [&](const sbe::sbe_message& __msg) {
    };
    output->on_incremental = [&](const sbe::sbe_message& __msg, uint64_t __timestamp) {
        clock_gettime(CLOCK_MONOTONIC, &_timer);
        results[index] = _timer.tv_nsec  - __timestamp;
        if(++index == 3000)
        {
            for(int i = 0; i < index; ++i)
            {
                std::cout << i <<","<< results[i] << std::endl;
            }
            index = 0;
        }
    };
    output->on_snapshot = [&](const sbe::sbe_message& __msg) {
    };
    auto config = b3::channel_config();
    config.instrument_def.address = "233.252.8.5";
    config.instrument_def.port = 30001;
    config.instrument_def.interface = "enp4s0";
    config.snapshot.address = "233.252.8.6";
    config.snapshot.port = 30002;
    config.snapshot.interface = "enp4s0";
    config.feed_a.address = "233.252.8.7";
    config.feed_a.port = 30003;
    config.feed_a.interface = "enp4s0";
    auto channel = b3::umdf::channel(config, output);
    try
    {
        if(!channel.start())
        {
            std::cout << "error join " << std::endl;
        }
        std::cout << "press any key to stop" << std::endl;
        int i = 0;
        std::cin >> i;
    }catch(...)
    {
        std::cout << "alguma expcetion" << std::endl;
    }
    return 0;
}