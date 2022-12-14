cmake_minimum_required(VERSION 3.24)
set(project_name mission-critical-sw-experiments)
project(${project_name})

set(generation_version 0)
set(major_version 0)
set(minor_version 3)
set(branch_version 0)
#option(BUILD_CLANG_TIDY "Build with clang-tidy" OFF)
option(BUILD_TESTS "Build tests" ON)

option(BUILD_WITH_ASAN "Build with ASan" ON)
option(BUILD_WITH_USAN "Build with USan" ON)
#option(BUILD_WITH_TSAN "Build with TSan" OFF)
#option(BUILD_WITH_FUZZ "Build with AFL" OFF)

#option(ABSL_PROPAGATE_CXX_STD "Build with AFL" ON)

set(CMAKE_VERBOSE_MAKEFILE true)

include_directories(${CMAKE_SOURCE_DIR})

list(APPEND CMAKE_PREFIX_PATH ${CMAKE_SOURCE_DIR}/cmake)
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

#if (BUILD_WITH_ASAN)
#    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-omit-frame-pointer -fsanitize=address")
#    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fno-omit-frame-pointer -fsanitize=address")
#    set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -fno-omit-frame-pointer -fsanitize=address")
#endif()
#
#if (BUILD_WITH_USAN)
#    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-omit-frame-pointer -fsanitize=undefined")
#    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fno-omit-frame-pointer -fsanitize=undefined")
#    set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -fno-omit-frame-pointer -fsanitize=undefined")
#endif()
#
#if (BUILD_WITH_TSAN)
#    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-omit-frame-pointer -fsanitize=thread")
#    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fno-omit-frame-pointer -fsanitize=thread")
#    set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -fno-omit-frame-pointer -fsanitize=thread")
#endif()

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

####################################################################################
####################################################################################
####################################################################################

find_package(benchmark REQUIRED)
include_directories(${benchmark_INCLUDE_DIRS})
link_libraries(${benchmark_LIBRARIES} pthread)

include_directories(${CMAKE_SOURCE_DIR}/3rdparty/farbot/include)

# It's big
add_subdirectory(3rdparty/gsl EXCLUDE_FROM_ALL)

#add_subdirectory(3rdparty/bounded-integer)  # Catch troubles
add_subdirectory(3rdparty/abseil-cpp)
#add_subdirectory(3rdparty/farbot)  # Can't connect like this
#find_package(iceoryx_hoofs REQUIRED)
add_subdirectory(3rdparty/SG14)

include_directories(${CMAKE_SOURCE_DIR}/3rdparty/bounded-integer/include/)
include_directories(${CMAKE_SOURCE_DIR}/3rdparty/bounded-integer/dependencies/numeric-traits/include/)
include_directories(${CMAKE_SOURCE_DIR}/3rdparty/bounded-integer/dependencies/operators/include/)

add_subdirectory(apps/safe)
add_subdirectory(apps/secure)
add_subdirectory(tests)
