

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

# Need build libc++ without exceptions
set(CMAKE_CXX_FLAGS "-fno-rtti -fno-exceptions ${CMAKE_CXX_FLAGS}")
add_library(safe_rt SHARED
        safe_eval.cpp)