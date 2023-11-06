#ifndef __cb_allocator__
#define __cb_allocator__

#include <cstddef>
#include <cstdint>
#include <memory>
#include <iostream>
#include <limits>
#include <new>
#include <type_traits>
#include <ctime>

#include "buffer.hpp"


namespace memory
{

template <class T> struct cb_allocator
{
    typedef T value_type;

    constexpr static std::uint64_t default_size = 1 << 18;

    cb_allocator() {
        _mask = default_size - 1;
        _memory_data = _allocate(default_size);
    }

    template<int Size, typename=std::enable_if_t<(Size != 0) && ((Size & (Size - 1)) == 0)>>
    cb_allocator() {
        _mask = Size - 1;
        _memory_data = _allocate(Size);
    }

    [[nodiscard]] T* allocate(std::size_t n) {
        if(_memory_data == nullptr || (_head - _tail >= _mask))
        {
            throw std::bad_array_new_length();
        }
        T* ret = &(_memory_data[_head & _mask]);
        _head += n;
        return  ret;
    }

    void deallocate([[maybe_unused]]T* p, std::size_t n) noexcept {
        _tail += n;
    }

    T* get_memory()
    {
        return _memory_data;
    }

  private:

    [[nodiscard]] T *_allocate(std::size_t n)
    {
        if (n > std::numeric_limits<std::size_t>::max() / sizeof(T))
        {
            throw std::bad_array_new_length();
        }

        if (auto p = static_cast<T *>(std::malloc(n * sizeof(T))))
        {
            return p;
        }

        throw std::bad_alloc();
    }

    void _deallocate(T *p, [[maybe_unused]]std::size_t n) noexcept
    {
        std::free(p);
    }

    void _report(T* p, std::size_t n, bool alloc = true) const
    {
        std::cout << (alloc ? "Alloc: " : "Dealloc: ") << sizeof(T) * n
                  << " bytes at " << std::hex << std::showbase
                  << reinterpret_cast<void*>(p) << std::dec << '\n';
    }

    void report(T* p, std::size_t n,  std::size_t pos, bool alloc = true) const
    {
        ptrdiff_t diff = p - _memory_data;
        std::cout << "In position " << pos <<  " diff: " <<  diff << (alloc ? " Alloc: " : " Dealloc: ") << sizeof(T) * n
                  << " bytes at " << std::hex << std::showbase
                  << reinterpret_cast<void*>(p) << std::dec << '\n';
    }

    T *_memory_data = nullptr;
    std::uint64_t _head = 0;
    std::uint64_t _tail = 0;
    std::uint64_t _mask = 0;

    public:
    template<class U>
    bool operator==(const cb_allocator<U> &) { return true; }

    template<class U>
    bool operator!=(const cb_allocator<U> &) { return false;}
};

[[maybe_unused]]static std::shared_ptr<buffer> allocate_shared_buffer(cb_allocator<buffer> &_alloc)
{
    buffer* raw_ptr = _alloc.allocate(1);
    auto ret = std::shared_ptr<buffer>(raw_ptr, [&_alloc](buffer* data) {
            _alloc.deallocate(data, 1);
    });

    return ret;
}

}; // namespace memory

#endif