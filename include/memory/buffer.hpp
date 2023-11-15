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

#ifndef MARKET_DATA_BUFFER_HPP
#define MARKET_DATA_BUFFER_HPP
#include <ctime>
#include <cstddef>
#include "allocator.hpp"

namespace memory
{
    struct buffer
    {
        buffer() = delete;

        buffer(allocator* __alloc, _Basic_block* __block) :
                _M_allocator(__alloc),
                _M_block_memory(__block) {}

        inline char * data()
        {
            return _M_block_memory->data();
        }

        inline std::size_t size()
        {
            return _M_block_memory->size();
        }

        inline void set_size(std::size_t __size)
        {
            _M_block_memory->size(__size);
        }

        inline const std::size_t capacity() const
        {
            return _M_block_memory->capacity();
        }

        inline const long created_at() const
        {
            return _M_block_memory->created_at();
        }

        inline void created_at(long __created_at)
        {
            _M_block_memory->created_at(__created_at);
        }

        inline const void release() const
        {
            _M_allocator->deallocate(_M_block_memory, 1);
        }

    private:
        _Basic_block *_M_block_memory;
        allocator* _M_allocator;
    };
} // namespace udp
#endif //MARKET_DATA_BUFFER_HPP
