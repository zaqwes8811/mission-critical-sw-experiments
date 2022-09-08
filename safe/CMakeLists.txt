

# Info
#   https://www.appinf.com/download/SafetyCriticalC++.pdf
#   "Writing Safety Critical Automotive Software for High Perf AI Hardware - Michael Wong - CppCon 2019"
#   https://stackoverflow.com/questions/51184557/testing-c17-in-safety-critical-systems
#
# Design in general
#   https://solwitsa.medium.com/guidelines-for-developing-safety-critical-systems-387511aee441
#
# Spaces
#   https://solwit.com/en/innovation-acceleration/safety-critical-systems/
#
# Multiprocessor
#   https://solwitsa.medium.com/multi-processor-solutions-in-safety-critical-systems-16a8036fe24a
#
# Z3
#   https://liu.diva-portal.org/smash/get/diva2:1366444/FULLTEXT01.pdf

# General
#   https://smallbusinessprogramming.com/safety-critical-software-15-things-every-developer-should-know/
#   "Safety-critical software systems are often embedded, distributed systems"
# Book
#   https://d-nb.info/1007034963/34

# Disable heap
#   https://stackoverflow.com/questions/40130374/gcc-how-to-disable-heap-usage-entirely-on-an-mcu

# Need build libc++ without exceptions
set(CMAKE_CXX_FLAGS "-fno-rtti -fno-exceptions -Wall -Werror -Wl,--wrap=malloc ${CMAKE_CXX_FLAGS}")
add_library(safe_rt SHARED
        safe_eval.cpp)

add_executable(app test_nomalloc.cpp)