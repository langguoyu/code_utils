cmake_minimum_required(VERSION 3.16)
project(test)

message(CMAKE_CXX_FLAGS=${CMAKE_CXX_FLAGS})
set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} "-Wl,--wrap=malloc -Wl,--wrap=free -Wl,--wrap=foo -Wl,--wrap=_Znwm -Wl,--wrap=_ZdlPv")
message(CMAKE_CXX_FLAGS=${CMAKE_CXX_FLAGS})

add_library(wrap SHARED wrap.cc)

add_executable(test test.cc foo.cc)
target_link_libraries(test PUBLIC wrap)
