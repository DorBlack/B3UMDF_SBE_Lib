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
#include <vector>

namespace memory
{
    class _Basic_block
    {
    public:
        _Basic_block() :
                _M_in_using(false),
                _M_data_size(0x00)
        {
            memset(_M_data, 0x00, _S_size);
        }

        _Basic_block(_Basic_block& __other) noexcept :
                _M_in_using(__other._M_in_using),
                _M_data_size(__other._M_data_size)
        {
            std::memcpy(_M_data, __other._M_data, __other._M_data_size);
        }

        _Basic_block(const _Basic_block& __other) noexcept :
                _M_in_using(__other._M_in_using),
                _M_data_size(__other._M_data_size)
        {
            std::memcpy(_M_data, __other._M_data, __other._M_data_size);
        }

        _Basic_block(_Basic_block&& other) = delete;
        _Basic_block(const _Basic_block&& other) = delete;
        _Basic_block& operator=(_Basic_block& other) = delete;
        _Basic_block& operator=(_Basic_block&& other) = delete;
        _Basic_block& operator=(const _Basic_block& other) = delete;
        _Basic_block& operator=(const _Basic_block&& other) = delete;

        const std::size_t capacity() const
        {
            return _S_size;
        }

        const std::size_t size() const
        {
            return _M_data_size;
        }

        void size(std::size_t __size)
        {
            _M_data_size = __size;
        }

        inline const bool in_using() const
        {
            return _M_in_using;
        }

        inline void in_using(bool __in_use)
        {
            _M_in_using = __in_use;
        }

        char* data()
        {
            return  _M_data;
        }

        const long created_at() const
        {
            return _M_created_at;
        }

        void created_at(long __created_at) {
            _M_created_at = __created_at;
        }
    private:
        constexpr static std::size_t _S_size = 1400;
        bool _M_in_using;
        long _M_created_at;
        std::size_t _M_data_size;
        char _M_data[_S_size];

    };

class allocator
{
    constexpr static std::uint64_t _S_default_size = 1 << 18;
public:
    allocator() {
        _M_mask = _S_default_size - 1;
    }

    [[nodiscard]] _Basic_block* allocate(std::size_t __size) {
        if(_M_memory_data == nullptr || (_M_head - _M_tail >= _M_mask))
        {
           std::cout << "full queue" << std::endl;
            throw std::bad_array_new_length();
        }
        _Basic_block* _ret = &(_M_memory_data[_M_head & _M_mask]);

        if(_ret->in_using())
        {
            std::cout << "erro no algo" << std::endl;
            throw std::bad_array_new_length();
        }

        _ret->in_using(true);
        _M_head += __size;
        return  _ret;
    }

    void deallocate([[maybe_unused]]_Basic_block* p, std::size_t n) noexcept {
        p->in_using(false);
        _M_tail += n;
//        std::cout << "dealoccate " << std::boolalpha << p->in_using() << std::noboolalpha << " queue size: " << _M_head - _M_tail << std::endl;
    }

    _Basic_block* get_memory()
    {
        return _M_memory_data;
    }

    template<typename T>
        static T
        allocate_obj(allocator* __allocator)
    {
        auto _ptr = __allocator->allocate(1);
        return T(__allocator, _ptr);
    }

    template<typename T>
    static std::shared_ptr<T>
    make_shared_ptr(allocator* __allocator)
    {
        auto _ptr = __allocator->allocate(1);
        std::shared_ptr<T> ret(new T(__allocator, _ptr), [](auto __ptr) {__ptr->release();});
        return ret;
    }


  private:

    [[nodiscard]] _Basic_block *_allocate(std::size_t n)
    {
        if (n > std::numeric_limits<std::size_t>::max() / sizeof(_Basic_block))
        {
            throw std::bad_array_new_length();
        }

        if (auto p = static_cast<_Basic_block *>(std::malloc(n * sizeof(_Basic_block))))
        {
            return p;
        }

        throw std::bad_alloc();
    }

    void _deallocate(_Basic_block *p, [[maybe_unused]]std::size_t n) noexcept
    {
        std::free(p);
    }

    void _report(_Basic_block* p, std::size_t n, bool alloc = true) const
    {
        std::cout << (alloc ? "Alloc: " : "Dealloc: ") << sizeof(_Basic_block) * n
                  << " bytes at " << std::hex << std::showbase
                  << reinterpret_cast<void*>(p) << std::dec << '\n';
    }

    void report(_Basic_block* p, std::size_t n,  std::size_t pos, bool alloc = true) const
    {
        ptrdiff_t diff = p - _M_memory_data;
        std::cout << "In position " << pos <<  " diff: " <<  diff << (alloc ? " Alloc: " : " Dealloc: ") << sizeof(_Basic_block) * n
                  << " bytes at " << std::hex << std::showbase
                  << reinterpret_cast<void*>(p) << std::dec << '\n';
    }

    _Basic_block _M_memory_data[_S_default_size];
    std::uint64_t _M_head = 0;
    std::uint64_t _M_tail = 0;
    std::uint64_t _M_mask = 0;
};
}; // namespace memory

#endif