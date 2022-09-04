#include <gtest/gtest.h>

#include <iostream>
#include <string>
#include <vector>


int main(int argc, char *argv[]) {
    testing::InitGoogleTest(&argc, argv);
    testing::GTEST_FLAG(print_time) = true;
    return RUN_ALL_TESTS();
}
