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
// Created by scien on 05/11/2023.
//

#ifndef B3MKTDATA_CACHE_HPP
#define B3MKTDATA_CACHE_HPP

#include <atomic>
#include <optional>

namespace memory {

template<typename Ty, int Pow = 10>
struct st_cache {

    st_cache() = default;

    void input(Ty value)
    {
        auto pos = _head & _mask;
        _cache[pos] = value;
        _head.fetch_add(1);
    }

    std::optional<Ty> try_get_next()
    {
        if (_head.load()>_tail) {
            return _cache[_tail++ & _mask];
        }
        else {
            return std::nullopt;
        }
    }

private:

    static constexpr std::uint64_t _mask = (1 << Pow)-1;
    std::uint64_t _tail = 0;
    std::atomic_uint64_t _head = 0;
    Ty _cache[1 << Pow];
};
}
#endif //B3MKTDATA_CACHE_HPP
