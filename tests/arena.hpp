//
// Created by zaqwes on 16.09.2022.
//

#pragma once

#include <iostream>
#include <vector>
#include <memory>
#include <cstring>
#include <cassert>
#include <cstddef>

class VectorBasedArena {
public:
    explicit VectorBasedArena(const size_t buf_len) : buf_len_{buf_len}, underlying_storage_(buf_len, 0xae) {
        buf = underlying_storage_.data();
    }

    // Idea: Max alignment is pessimistic
    [[nodiscard]] void *alloc(const size_t size, const size_t align = alignof(std::max_align_t)) {
        std::cout << "VectorBasedArena: alloc req: " << size << std::endl;
        auto a = this;
        // Align 'curr_offset' forward to the specified alignment
        const uintptr_t curr_ptr = (uintptr_t)a->buf + (uintptr_t)a->curr_offset;
        const uintptr_t offset = align_forward(curr_ptr, align) - (uintptr_t)a->buf;  // Change to relative offset

        // Check to see if the backing memory has space left
        if (offset + size <= a->buf_len_) {
            a->curr_offset = offset + size;

            // Zero new memory by default
            void *ptr = &a->buf[offset];
            memset(ptr, 0, size);

            return ptr;
        }
        // Return NULL if the arena is out of memory (or handle differently)
        return nullptr;
    }

    [[nodiscard]] const auto &storage() const { return underlying_storage_; }

private:
    [[nodiscard]] static constexpr bool is_power_of_two(const uintptr_t x) { return (x & (x - 1)) == 0; }

    [[nodiscard]] static constexpr uintptr_t align_forward(const uintptr_t ptr, const size_t align) {
        assert(is_power_of_two(align));

        uintptr_t p = ptr;
        const auto a = static_cast<uintptr_t>(align);

        // Same as (p % a) but faster as 'a' is a power of two
        const uintptr_t modulo = p & (a - 1);

        if (modulo != 0) {
            // If 'p' address is not aligned, push the address to the
            // next value which is aligned
            p += a - modulo;
        }
        return p;
    }

    unsigned char *buf{};
    const size_t buf_len_{};
    size_t curr_offset{0};
    std::vector<unsigned char> underlying_storage_;
};

template <class T>
class ArenaAllocator {
    // https://stackoverflow.com/questions/48061522/create-the-simplest-allocator-with-two-template-arguments
public:
    typedef T value_type;
    typedef value_type *pointer;
    typedef const value_type *const_pointer;
    typedef value_type &reference;
    typedef const value_type &const_reference;
    typedef std::size_t size_type;
    typedef std::ptrdiff_t difference_type;
    using propagate_on_container_move_assignment = std::true_type;
    using is_always_equal = std::true_type;

    ArenaAllocator() = default;

    explicit ArenaAllocator(VectorBasedArena *a) noexcept : a_{a} {}

    ArenaAllocator(const ArenaAllocator &) = default;

    template <typename U>
    explicit ArenaAllocator(const ArenaAllocator<U> &rhs) noexcept {
        a_ = rhs.a_;
    }

    inline pointer allocate(size_t num) {
        assert(a_);
        auto ptr = a_->alloc(num * sizeof(T), alignof(T));
        if (!ptr) throw std::bad_alloc();

        // https://en.cppreference.com/w/cpp/utility/launder
        return std::launder(reinterpret_cast<pointer>(ptr));
    }

    void deallocate(pointer p, size_t num) noexcept {}

    template <typename U>
    struct rebind {
        typedef ArenaAllocator<U> other;
    };

    VectorBasedArena *a_ = nullptr;
};

template <class T1, class T2>
bool operator==(const ArenaAllocator<T1> &, const ArenaAllocator<T2> &) noexcept {
    return true;
}

template <class T1, class T2>
bool operator!=(const ArenaAllocator<T1> &, const ArenaAllocator<T2> &) noexcept {
    return false;
}

static auto global_arena_deleter = [](auto ptr) {
    // TODO() Will be destructor called?
    //   std::is_trivially_destructible - how to escape destruction
    //
    // https://stackoverflow.com/questions/41897418/significance-of-trivial-destruction
    std::destroy_at(std::launder(ptr));
};

using AllocatorStr = ArenaAllocator<char>;
using ArenaString = std::basic_string<char, std::char_traits<char>, AllocatorStr>;
using AllocatorIntStrPair = ArenaAllocator<std::pair<const int, ArenaString>>;

template <typename T>
using ArenaVector = std::vector<T, ArenaAllocator<T>>;
