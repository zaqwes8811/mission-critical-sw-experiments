//
// Created by zaqwes on 16.09.2022.
//

#pragma once

#include <cassert>
#include <cstddef>
#include <cstring>
#include <iostream>
#include <memory>
#include <vector>

#include "apps/safe/arena.hpp"

class VectorBasedArena {
public:
    explicit VectorBasedArena(const size_t buf_len)
        : underlying_storage_(buf_len, 0xae), arena_{underlying_storage_.data(), buf_len} {}

    [[nodiscard]] const auto &storage() const { return underlying_storage_; }

    [[nodiscard]] void *alloc(const size_t size, const size_t align = alignof(std::max_align_t)) {
        arena_.alloc(size, align);
    }

private:
    std::vector<unsigned char> underlying_storage_;

    Arena arena_;
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
