//
// Created by zaqwes on 14.09.2022.
//

// Vector strings of known max size in single peace of memory

// VectorBasedArena
//   https://www.gingerbill.org/article/2019/02/08/memory-allocation-strategies-002/
// std::pmr
// vector control / set? | str-head-1/str-body-1 | str-head-2/str-body-2
// https://www.gingerbill.org/article/2019/02/08/memory-allocation-strategies-002/

#include <gtest/gtest.h>

#include <iomanip>
#include <unordered_set>

#include "arena.hpp"

//========================================================================================
//========================================================================================
//========================================================================================

namespace {
auto &o = std::cout;

void dump(const std::vector<unsigned char> &arena_buffer) {
    size_t ptr = 0;

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

template <typename T>
using ArenaVector = std::vector<T, ArenaAllocator<T>>;

using String = std::basic_string<char, std::char_traits<char>, ArenaAllocator<char>>;
// using String = std::basic_string<char, std::char_traits<char>, std::allocator<char>>;

TEST(TightAllocTest, ArenaUnderlyingApi) {
    auto arena = VectorBasedArena{256 + 64};
    dump(arena.storage());

    auto allocator = ArenaAllocator<int>{&arena};
    //    {
    //        auto v = ArenaVector<int>(allocator);
    //        v.push_back(16);
    //    }
    //    dump(arena_buffer);

    {
        using A = ArenaVector<int>;
        auto a = (A *)arena.alloc(sizeof(A), alignof(A));
        new (a) A(allocator);
        a->reserve(5);
        a->push_back(9);
    }
    dump(arena.storage());

    {
        //        using A = std::unordered_set<int, std::hash<int>, std::equal_to<>, ArenaAllocator<int>>;
        using A = std::set<int, std::less<>, ArenaAllocator<int>>;
        auto a = (A *)arena.alloc(sizeof(A), alignof(A));
        ASSERT_NE(a, nullptr);

        auto raw_ptr = new (a) A(allocator);
        ASSERT_NE(raw_ptr, nullptr);

        auto ptr = std::unique_ptr<A, decltype(global_arena_deleter)>(raw_ptr, global_arena_deleter);

        a->insert(9);
        a->insert(8);
    }
    dump(arena.storage());

    // String
    // https://stackoverflow.com/questions/37502974/stdstring-with-a-custom-allocator
    //    {
    //        auto allocator = ArenaAllocator<char>{&arena};
    //
    //        using A = String;
    //        auto a = (A *)alloc(&arena, sizeof(A));
    //        ASSERT_NE(a, nullptr);
    //        dump(arena_buffer);
    //        new (a) A(allocator);
    //        a->reserve(32);
    //        *a += "1234";
    //        //        *a += "1234567812345678";
    //    }
    dump(arena.storage());
}

TEST(TightAllocTest, CustomeAllocatorForUPtr) {
    // https://stackoverflow.com/questions/33845132/using-stdunique-ptr-with-allocators
    auto arena = VectorBasedArena{256 + 64};
    dump(arena.storage());

    auto allocator = ArenaAllocator<int>{&arena};

    using A = std::set<int, std::less<>, ArenaAllocator<int>>;
    auto a = (A *)arena.alloc(sizeof(A), alignof(A));
    ASSERT_NE(a, nullptr);

    auto raw_ptr = new (std::launder(a)) A(allocator);
    ASSERT_NE(raw_ptr, nullptr);

    // https://stackoverflow.com/questions/17328454/calling-destructor-with-decltype-and-or-stdremove-reference

    auto ptr = std::unique_ptr<A, decltype(global_arena_deleter)>(raw_ptr, global_arena_deleter);
}