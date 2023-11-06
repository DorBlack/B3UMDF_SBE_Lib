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

#ifndef MARKET_DATA_CHANNEL_ENGINE_HPP
#define MARKET_DATA_CHANNEL_ENGINE_HPP

#include "b3/protocol/sbe_message.hpp"
#include "iengine.hpp"

namespace b3::umdf
{

template<typename Buffer>
struct umdf_b3_sbe_engine {

    using protocol_type = protocol::sbe::message<Buffer>;
    using notify_type = engine::engine_notification<protocol_type>;

    umdf_b3_sbe_engine(std::shared_ptr<notify_type> ny) {

    }

    void instrument_def(std::shared_ptr<Buffer> buffer) {

    }

    void snapshot(std::shared_ptr<Buffer> buffer) {

    }

    bool incremental(std::shared_ptr<Buffer> buffer) {
        return true;
    }
};
}

#endif //MARKET_DATA_CHANNEL_ENGINE_HPP
