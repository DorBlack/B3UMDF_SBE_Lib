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

#include "gtest/gtest.h"
#include "memory/st_cache.hpp"


TEST(StCacheTest, CacheTest)
{
    auto intCache = memory::st_cache<int, 5>();
    intCache.input(1);
    auto result = intCache.try_get_next();
    ASSERT_TRUE(result);
    EXPECT_EQ(result.value(), 1);

    auto ptrIntCache = memory::st_cache<std::shared_ptr<int>>();

    ptrIntCache.input(std::make_shared<int>(10));
    auto ptrResult = ptrIntCache.try_get_next();
    ASSERT_TRUE(ptrResult);
    EXPECT_EQ(*(ptrResult.value()), 10);

    auto ptrResultNull = ptrIntCache.try_get_next();
    ASSERT_FALSE(ptrResultNull);
}