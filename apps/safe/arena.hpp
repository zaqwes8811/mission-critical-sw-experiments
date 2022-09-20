//
// Created by zaqwes on 20.09.2022.
//

#pragma once

#include <cassert>
#include <cstddef>
#include <cstring>
#include <memory>

// NoExcept compatible
// Can use external storage

class Arena {
public:
    Arena(unsigned char *buf, const size_t buf_len) : buf_{buf}, buf_len_{buf_len} {}

    [[nodiscard]] void *alloc(const size_t size, const size_t align = alignof(std::max_align_t)) {
        auto a = this;
        // Align 'curr_offset' forward to the specified alignment
        const uintptr_t curr_ptr = (uintptr_t)a->buf_ + (uintptr_t)a->curr_offset;
        const uintptr_t offset = align_forward(curr_ptr, align) - (uintptr_t)a->buf_;  // Change to relative offset

        // Check to see if the backing memory has space left
        if (offset + size <= a->buf_len_) {
            a->curr_offset = offset + size;

            // Zero new memory by default
            void *ptr = &a->buf_[offset];
            memset(ptr, 0, size);  // Do we need it really?

            return ptr;
        }
        // Return NULL if the arena is out of memory (or handle differently)
        return nullptr;
    }

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

    unsigned char *buf_{};
    const size_t buf_len_{};
    size_t curr_offset{0};
};