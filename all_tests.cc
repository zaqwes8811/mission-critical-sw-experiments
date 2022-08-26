#include <gtest/gtest.h>

#include <iostream>
#include <string>
#include <vector>

int *g;

// ASAN_OPTIONS=detect_stack_use_after_return=1
namespace {
void LeakLocal() {
    int local;
    g = &local;
}

int *get() {
    int local;
    return &local;
}
}  // namespace

int main(int argc, char *argv[]) {
    // int *a = new int[100];
    // delete[] a;
    // int b =
    // return a[9];
    // int k = *get();
    // return k+100;
    // std::vector<int> r;
    // r.reserve(90);
    // return r[9];
    // int i = *get();

    // LeakLocal();
    // return *g;

    // Run
    testing::InitGoogleTest(&argc, argv);
    testing::GTEST_FLAG(print_time) = true;
    return RUN_ALL_TESTS();
}
