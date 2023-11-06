add_subdirectory("${PROJECT_SOURCE_DIR}/submodules/benchmark" "submodules/benchmark")

add_executable(MemoryAllocatorBench)
target_compile_options(MemoryAllocatorBench PUBLIC -fno-omit-frame-pointer )
target_sources(MemoryAllocatorBench PRIVATE memory_allocator_bench.cpp)
target_link_libraries(MemoryAllocatorBench PUBLIC benchmark::benchmark b3_umdf)
