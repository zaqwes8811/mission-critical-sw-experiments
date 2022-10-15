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

set(CMAKE_VERBOSE_MAKEFILE true)

include_directories(${CMAKE_SOURCE_DIR})

list(APPEND CMAKE_PREFIX_PATH ${CMAKE_SOURCE_DIR}/cmake)
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

if (BUILD_WITH_ASAN)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-omit-frame-pointer -fsanitize=address")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fno-omit-frame-pointer -fsanitize=address")
    set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -fno-omit-frame-pointer -fsanitize=address")
endif()

if (BUILD_WITH_USAN)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-omit-frame-pointer -fsanitize=undefined")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fno-omit-frame-pointer -fsanitize=undefined")
    set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -fno-omit-frame-pointer -fsanitize=undefined")
endif()
#
#if (BUILD_WITH_TSAN)
#    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-omit-frame-pointer -fsanitize=thread")
#    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fno-omit-frame-pointer -fsanitize=thread")
#    set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -fno-omit-frame-pointer -fsanitize=thread")
#endif()

#if (PLATFORM_TYPE STREQUAL "X86_64_TORCH_LEGACY")
#    set(CMAKE_CXX_STANDARD 14)
#else ()
#set(CMAKE_CXX_STANDARD 17)
#set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
#endif ()

#if (CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCXX)
#    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -O3")
#endif ()

####################################################################################
####################################################################################
####################################################################################

# It's big
#add_subdirectory(3rdparty/bounded-integer)

find_package(GTest REQUIRED)

include_directories(${GTEST_INCLUDE_DIRS})

#get_cmake_property(_variableNames VARIABLES)
#list (SORT _variableNames)
#foreach (_variableName ${_variableNames})
#    message(STATUS "${_variableName}=${${_variableName}}")
#endforeach()

find_package(benchmark REQUIRED)
include_directories(${benchmark_INCLUDE_DIRS})
link_libraries(${benchmark_LIBRARIES} pthread)

include_directories(${CMAKE_SOURCE_DIR}/3rdparty/farbot/include)

find_package(GTest REQUIRED)
find_package(iceoryx_hoofs REQUIRED)

include_directories(${CMAKE_SOURCE_DIR}/3rdparty/bounded-integer/include/)
include_directories(${CMAKE_SOURCE_DIR}/3rdparty/bounded-integer/dependencies/numeric-traits/include/)
include_directories(${CMAKE_SOURCE_DIR}/3rdparty/bounded-integer/dependencies/operators/include/)

add_subdirectory(apps/safe)
add_subdirectory(apps/secure)
add_subdirectory(tests)
