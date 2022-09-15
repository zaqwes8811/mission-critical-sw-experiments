//
// Created by zaqwes on 14.09.2022.
//

#include <gtest/gtest.h>

#include <iomanip>

// Vector strings of known max size in single peace of memory

// Arena
//   https://www.gingerbill.org/article/2019/02/08/memory-allocation-strategies-002/

// std::pmr

// vector control / set? | str-head-1/str-body-1 | str-head-2/str-body-2

// https://www.gingerbill.org/article/2019/02/08/memory-allocation-strategies-002/
auto &o = std::cout;
// auto& nl =  std::endl;

namespace arena {
constexpr bool is_power_of_two(uintptr_t x) { return (x & (x - 1)) == 0; }

constexpr uintptr_t align_forward(uintptr_t ptr, size_t align) {
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

struct Arena {
    Arena(unsigned char *buf_, size_t buf_len_) : buf{buf_}, buf_len{buf_len_} {
        o << "Arena: start_addr: " << std::hex << static_cast<void *>(buf_) << std::dec << std::endl;
    }
    unsigned char *buf{};
    size_t buf_len{};
    size_t curr_offset{0};
};

void *arena_alloc_align(Arena *a, size_t size, size_t align) {
    o << "size:" << size << " align:" << align << std::endl;
    // Align 'curr_offset' forward to the specified alignment
    const uintptr_t curr_ptr = (uintptr_t)a->buf + (uintptr_t)a->curr_offset;

    const uintptr_t offset = align_forward(curr_ptr, align) - (uintptr_t)a->buf;  // Change to relative offset
    o << std::hex << align_forward(curr_ptr, align) << " curr:" << curr_ptr << std::dec << " offset:" << offset
      << std::endl;

    // Check to see if the backing memory has space left
    if (offset + size <= a->buf_len) {
        a->curr_offset = offset + size;

        // Zero new memory by default
        void *ptr = &a->buf[offset];
        memset(ptr, 0, size);

        o << "Arena: alloc_addr: " << std::hex << static_cast<void *>(ptr) << std::dec << std::endl;
        return ptr;
    }
    // Return NULL if the arena is out of memory (or handle differently)
    return nullptr;
}

// Because C doesn't have default parameters
// Idea: Max alignment is pessimistic
void *arena_alloc(Arena *a, size_t size) { return arena_alloc_align(a, size, alignof(std::max_align_t)); }

}  // namespace arena

namespace {
void dump(const std::vector<unsigned char> &arena_buffer) {
    int ptr = 0;

    o << std::setw(4) << std::setfill('0') << ptr << ": ";
    for (auto e : arena_buffer) {
        o << std::hex << "0x" << std::setw(2) << std::setfill('0') << static_cast<int>(e) << " ";

        ++ptr;
        if (ptr % 8 == 0) {
            o << " ";
        }
        if (ptr % 16 == 0) {
            o << std::endl;
            if (ptr != arena_buffer.size()) {
                o << std::dec << std::setw(4) << std::setfill('0') << ptr << ": ";
            }
        }
    }
    o << std::dec << std::endl;
}
}  // namespace

struct TestStr {
    int flag{1};
};

TEST(TightAllocTest, ArenaUnderlyingApi) {
    using namespace arena;
    auto arena_buffer = std::vector<unsigned char>(128, 0);
    auto arena = Arena{arena_buffer.data(), arena_buffer.size()};
    dump(arena_buffer);

    {
        using A = TestStr;//std::vector<int>;
        auto a = (A *)arena_alloc(&arena, sizeof(A));
        new (a) A();//3, 9);
        //        a->~A();
    }
    dump(arena_buffer);

    {
        using A = TestStr;  // std::set<int>;
        auto a = (A *)arena_alloc(&arena, sizeof(A));
        ASSERT_NE(a, nullptr);

        o << "Test: alloc_addr: " << std::hex << static_cast<void *>(a) << std::dec << std::endl;

        new (a) A();
        //        a->insert(1);
        //        a->~A();
    }
    dump(arena_buffer);
}