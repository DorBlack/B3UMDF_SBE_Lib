#include "benchmark/benchmark.h"
#include "memory/allocator.hpp"

[[maybe_unused]]static void BM_CBAllocatorRaw(benchmark::State& state) {

    auto alloc = memory::cb_allocator<memory::buffer>();

    for(auto _ : state)
    {
        auto raw = alloc.allocate(1);
        alloc.deallocate(raw , 1);
    }
}

[[maybe_unused]]static void BM_CBAllocatorMakeShared(benchmark::State& state) {

    auto alloc = memory::cb_allocator<memory::buffer>();

    for(auto _ : state)
    {
        [[maybe_unused]]auto shared_bf = memory::allocate_shared_buffer(alloc);
    }
}

[[maybe_unused]]static void BM_MakeShared(benchmark::State& state) {


    for(auto _ : state)
    {
        [[maybe_unused]]auto shared_bf = std::make_shared<memory::buffer>();
    }
}

[[maybe_unused]]static void BM_CBAllocatorSharedAssing(benchmark::State& state) {


    auto alloc = memory::cb_allocator<memory::buffer>();
    [[maybe_unused]]auto shared_bf = memory::allocate_shared_buffer(alloc);
    for(auto _ : state)
    {
        [[maybe_unused]]auto assin = shared_bf;
    }
}

[[maybe_unused]]static void BM_SharedPTrMove(benchmark::State& state) {
    for(auto _ : state)
    {
        auto shared = std::make_shared<memory::buffer>();
        [[maybe_unused]] auto mv = std::move(shared);
    }
}

[[maybe_unused]]static void BM_SharedPTrCopy(benchmark::State& state) {
    for(auto _ : state)
    {
        auto shared = std::make_shared<memory::buffer>();
        [[maybe_unused]] auto mv = shared;
    }
}

[[maybe_unused]]static void BM_SBEProtocolMessage(benchmark::State& state) {

    for(auto _ : state)
    {
        auto shared = std::make_shared<memory::buffer>();
        [[maybe_unused]] auto mv = shared;
    }
}

BENCHMARK(BM_SharedPTrMove);
BENCHMARK(BM_SharedPTrCopy);
BENCHMARK(BM_CBAllocatorRaw);
BENCHMARK(BM_CBAllocatorMakeShared);
BENCHMARK(BM_MakeShared);
BENCHMARK(BM_CBAllocatorSharedAssing);
BENCHMARK_MAIN();