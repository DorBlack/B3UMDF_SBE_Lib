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
// Created by scien on 29/10/2023.
//

#ifndef MARKET_DATA_IENGINE_HPP
#define MARKET_DATA_IENGINE_HPP

/**
 * IEgine: this file all requirements to implement a new engine to process
 * market data message.
 *
 * IEgine is a template implementation
 *
 */

#include <memory>
#include <functional>

namespace b3::engine
{
    enum class NotificationType
    {
        InstrumentDefinition,
        Snapshot,
        IncrementalTransition,
        Incremental,
        IncrementalGap,
        IncrementalReset,
        IncrementalFullReset
    };

    template<typename Protocol>
    struct engine_notification
    {
        std::function<void(std::shared_ptr<Protocol>)> on_incremental;
        std::function<void(std::shared_ptr<Protocol>)> on_snapshot;
        std::function<void(std::shared_ptr<Protocol>)> on_instrument_def;
        std::function<void(NotificationType)> on_notification;
    };



}
#endif //MARKET_DATA_IENGINE_HPP
