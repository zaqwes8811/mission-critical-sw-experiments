
# TODO() Add UBSan + Debug build

find_package(GTest REQUIRED)
#find_package(absl REQUIRED)

set(CMAKE_CXX_FLAGS "-Wall -fconcepts-diagnostics-depth=4  ${CMAKE_CXX_FLAGS}")

add_executable(tests-ut
        ${CMAKE_SOURCE_DIR}/tests/all_tests.cc
        ${CMAKE_SOURCE_DIR}/tests/iceoyx_hoofs_test.cpp
        ${CMAKE_SOURCE_DIR}/tests/models_test.cpp
        ${CMAKE_SOURCE_DIR}/tests/rt_parts_test.cpp
        numa_aware_allocator_test.cpp
        tight_alloc_test.cpp
        custome_primitives_test.cpp
        abseil_error_handling_test.cpp
        ${CMAKE_SOURCE_DIR}/modules/rtos_rte_cp/RT-Clock/posix_clock.c
        compile_time_graph.cpp
        cpp_concept_test.cpp
        cpp_ranges_test.cpp
        gsl_explore_test.cpp
        alg_design_test.cpp
        )


target_link_libraries(tests-ut
        ${GTEST_BOTH_LIBRARIES}
        pthread dl m rt
        )

get_target_property(absl_status_inc absl_status INCLUDE_DIRECTORIES)
target_include_directories(tests-ut PUBLIC ${absl_status_inc}
        ${CMAKE_SOURCE_DIR}/3rdparty/gsl/include
        )


get_target_property(absl_status_libs absl_status LINK_LIBRARIES)
get_target_property(absl_statusor_libs absl_statusor LINK_LIBRARIES)
target_link_libraries(tests-ut ${absl_status_libs} ${absl_statusor_libs}
        absl::statusor  # Had to add it manually
        )
