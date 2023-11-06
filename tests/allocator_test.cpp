#include <iostream>
#include "gtest/gtest.h"
#include "memory/allocator.hpp"

TEST(socket_allocator, UsingSharedPtr)
{
    auto alloc = memory::cb_allocator<memory::buffer>();

    {
        [[maybe_unused]]auto shared_bf = memory::allocate_shared_buffer(alloc);
    }

    {
        [[maybe_unused]]auto shared_bf = memory::allocate_shared_buffer(alloc);
    }

    {
        [[maybe_unused]]auto shared_bf = memory::allocate_shared_buffer(alloc);
    }
}

TEST(socket_allocator, UsingSharedPtr2)
{
    auto alloc = memory::cb_allocator<memory::buffer>();
    {
        [[maybe_unused]]auto shared_bf = memory::allocate_shared_buffer(alloc);
        {
            auto b1 = shared_bf;
            {
                auto b2 = b1;
                {
                    auto b3 = b2;
                }
            }
        }
    }
}
