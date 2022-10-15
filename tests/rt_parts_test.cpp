//
// Created by zaqwes on 07.01.2022.
//

// https://www.youtube.com/watch?v=Tof5pRedskI&ab_channel=CppCon

#include <gtest/gtest.h>

#include <array>
#include <memory_resource>
#include <span>

#include "3rdparty/SG14/SG14/inplace_function.h"
#include "3rdparty/bounded-integer/include/containers/static_vector.hpp"

std::array<float, 1024> stack_mem;

void process() {
    std::pmr::monotonic_buffer_resource monotonic_buffer(stack_mem.data(), stack_mem.size(),
                                                         std::pmr::null_memory_resource());

    using allocator_t = std::pmr::polymorphic_allocator<float>;
    allocator_t allocator(&monotonic_buffer);
    std::pmr::vector<float> my_vector(16, 0, allocator);
}

// inplace function

std::array<int, 64> a;

stdext::inplace_function<void(), 1024> no_op{[a = a] {}};

static_assert(std::is_nothrow_move_assignable_v<decltype(no_op)>, "A");

// Static vector
containers::static_vector<int, 128u> sv;

// https://www.modernescpp.com/index.php/c-17-avoid-copying-with-std-string-view

// Idea:
//   - heapless
//   - noexcept
template <size_t Capacity=16>
class fixed_capacity_string {
public:
    // TODO:
    // Span
    // https://stackoverflow.com/questions/70598724/transforming-a-string-view-in-place
    explicit fixed_capacity_string(std::string_view s) {
        if (s.size() > Capacity) {
            return;
        }

        s_ = std::string_view(buffer_.data(), s.size());
        v_ = std::span(buffer_.data(), s.size());
//        std::copy(s.begin(), s.end(), s_.begin());  // string_view is read_only
        std::copy(s.begin(), s.end(), v_.begin());

        std::cout << s_ << std::endl;
    }

private:
    bool constructed_{false};
    std::array<std::string_view::value_type, Capacity> buffer_;
    std::string_view s_;
    std::span<std::string_view::value_type> v_;
};

TEST(RtModules, FixedCapacityString) {
    std::string orig_str{"view"};
    fixed_capacity_string s{orig_str};
}
