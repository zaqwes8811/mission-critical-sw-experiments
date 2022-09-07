

# Info
#   https://www.appinf.com/download/SafetyCriticalC++.pdf
#   "Writing Safety Critical Automotive Software for High Perf AI Hardware - Michael Wong - CppCon 2019"
#   https://stackoverflow.com/questions/51184557/testing-c17-in-safety-critical-systems

# Need build libc++ without exceptions
set(CMAKE_CXX_FLAGS "-fno-rtti -fno-exceptions ${CMAKE_CXX_FLAGS}")
add_library(safe_rt SHARED
        safe_eval.cpp)