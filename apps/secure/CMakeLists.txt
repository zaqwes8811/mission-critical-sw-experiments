
# Need build libc++ without exceptions
#set(CMAKE_CXX_FLAGS "-fno-rtti -fno-exceptions -Wall -Werror -Wl,--wrap=malloc ${CMAKE_CXX_FLAGS}")
#add_library(safe_rt SHARED
#        safe_eval_test.cpp)

add_executable(secure cert_c.cpp)

