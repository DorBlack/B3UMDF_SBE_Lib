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

namespace memory
{
struct buffer
{
   char * data()
   {
       return _data;
   }

   std::size_t size()
   {
       return _size;
   }

   void set_size(std::size_t s)
   {
       _size = s;
   }

   std::size_t capacity()
   {
       return __size;
   }

   std::time_t created = std::time(0);
    std::size_t _size = 0;
private:
    constexpr static std::size_t __size = 1400;
    char _data[__size];
    std::byte _padding[632];
};
} // namespace udp
#endif //MARKET_DATA_BUFFER_HPP
