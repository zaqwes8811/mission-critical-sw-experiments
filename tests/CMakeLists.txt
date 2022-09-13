add_executable(rt_acs
        ${CMAKE_SOURCE_DIR}/tests/all_tests.cc
        ${CMAKE_SOURCE_DIR}/tests/iceoyx_hoofs_test.cpp
        ${CMAKE_SOURCE_DIR}/tests/models_test.cpp
        ${CMAKE_SOURCE_DIR}/tests/rt_parts_test.cpp
        numa_avare_allocator_test.cpp
        ${CMAKE_SOURCE_DIR}/modules/rtos_rte_cp/RT-Clock/posix_clock.c
        )


target_link_libraries(rt_acs
        ${GTEST_BOTH_LIBRARIES}
        pthread dl m rt
        )