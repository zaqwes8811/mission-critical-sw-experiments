//
// Created by zaqwes on 14.09.2022.
//

#include <gtest/gtest.h>

#include <iomanip>
#include <unordered_set>

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
    o << "Arena: size:" << size << " align:" << align << std::endl;
    // Align 'curr_offset' forward to the specified alignment
    const uintptr_t curr_ptr = (uintptr_t)a->buf + (uintptr_t)a->curr_offset;
    const uintptr_t offset = align_forward(curr_ptr, align) - (uintptr_t)a->buf;  // Change to relative offset

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

// https://stackoverflow.com/questions/48061522/create-the-simplest-allocator-with-two-template-arguments
template <class T>
class ArenaAllocator {
public:
    typedef T value_type;
    typedef value_type *pointer;
    typedef const value_type *const_pointer;
    typedef value_type &reference;
    typedef const value_type &const_reference;
    typedef std::size_t size_type;
    typedef std::ptrdiff_t difference_type;

    //    using value_type = T;
    using propagate_on_container_move_assignment = std::true_type;
    using is_always_equal = std::true_type;

    ArenaAllocator() = default;

    explicit ArenaAllocator(Arena *a) noexcept : a_{a} {}

    ArenaAllocator(const ArenaAllocator &) = default;

    template <typename U>
    explicit ArenaAllocator(const ArenaAllocator<U> &rhs) noexcept {
        a_ = rhs.a_;
    }

    inline pointer allocate(size_t num) {
        auto ptr = arena_alloc(a_, num * sizeof(T));
        //        auto ptr = malloc(num * sizeof(T));
        if (!ptr) throw std::bad_alloc();
        return reinterpret_cast<T *>(ptr);
    }

    void deallocate(pointer p, size_t num) noexcept {}

    template <typename U>
    struct rebind {
        typedef ArenaAllocator<U> other;
    };

    // private:
    Arena *a_ = nullptr;
};

template <class T1, class T2>
bool operator==(const ArenaAllocator<T1> &, const ArenaAllocator<T2> &) noexcept {
    return true;
}

template <class T1, class T2>
bool operator!=(const ArenaAllocator<T1> &, const ArenaAllocator<T2> &) noexcept {
    return false;
}
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

template <typename T>
using ArenaVector = std::vector<T, arena::ArenaAllocator<T>>;

using String = std::basic_string<char, std::char_traits<char>, arena::ArenaAllocator<char>>;
// using String = std::basic_string<char, std::char_traits<char>, std::allocator<char>>;

TEST(TightAllocTest, ArenaUnderlyingApi) {
    using namespace arena;
    auto arena_buffer = std::vector<unsigned char>(256, 0);
    auto arena = Arena{arena_buffer.data(), arena_buffer.size()};
    dump(arena_buffer);

    auto allocator = ArenaAllocator<int>{&arena};
    //    {
    //        auto v = ArenaVector<int>(allocator);
    //        v.push_back(16);
    //    }
    //    dump(arena_buffer);

    {
        using A = ArenaVector<int>;
        //        auto a = (A *)arena_alloc(&arena, sizeof(A));
        //        new (a) A(allocator);
        //        a->reserve(5);
        //        a->push_back(9);
    }
    dump(arena_buffer);

    {
        // TODO() Need rebind for allocator or something
        //        using A = std::unordered_set<int, std::hash<int>, std::equal_to<>, ArenaAllocator<int>>;
        using A = std::set<int, std::less<>, ArenaAllocator<int>>;  // Need extra work
        auto a = (A *)arena_alloc(&arena, sizeof(A));
        ASSERT_NE(a, nullptr);
        new (a) A(allocator);
        a->insert(9);
        a->insert(8);
    }
    dump(arena_buffer);

    // String
    // https://stackoverflow.com/questions/37502974/stdstring-with-a-custom-allocator
    //    {
    //        auto allocator = ArenaAllocator<char>{&arena};
    //
    //        using A = String;
    //        auto a = (A *)arena_alloc(&arena, sizeof(A));
    //        ASSERT_NE(a, nullptr);
    //        dump(arena_buffer);
    //        new (a) A(allocator);
    //        a->reserve(32);
    //        *a += "1234";
    //        //        *a += "1234567812345678";
    //    }
    dump(arena_buffer);
}