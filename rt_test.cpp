//
// Created by zaqwes on 07.01.2022.
//

// https://www.youtube.com/watch?v=Tof5pRedskI&ab_channel=CppCon

#include <gtest/gtest.h>

#include <array>
#include <memory_resource>

#include "3rdparty/SG14/SG14/inplace_function.h"

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
