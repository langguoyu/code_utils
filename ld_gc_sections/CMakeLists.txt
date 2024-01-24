project(demo)
cmake_minimum_required(VERSION 2.6)

set(TEST_1 "test_1")
set(TEST_2 "test_2")
set(TEST_3 "test_3")

add_library(${TEST_1} STATIC test_1.cpp)
set_target_properties(${TEST_1} PROPERTIES COMPILE_FLAGS "-fPIC -ffunction-sections -fdata-sections")
add_library(${TEST_1}_disable STATIC test_1.cpp)

add_library(${TEST_2} SHARED test_2.cpp)
target_link_libraries(${TEST_2} LINK_PRIVATE ${TEST_1})
set_target_properties(${TEST_2} PROPERTIES COMPILE_FLAGS "-fvisibility=hidden -ffunction-sections -fdata-sections")
set_target_properties(${TEST_2} PROPERTIES LINK_FLAGS "-Wl,-exclude-libs=ALL,-gc-sections")
add_library(${TEST_2}_disable SHARED test_2.cpp)
target_link_libraries(${TEST_2}_disable LINK_PRIVATE ${TEST_1})

add_executable(${TEST_3} test_3.cpp)
target_link_libraries(${TEST_3} LINK_PRIVATE ${TEST_1})
set_target_properties(${TEST_3} PROPERTIES LINK_FLAGS "-Wl,-exclude-libs=ALL,-gc-sections")
add_executable(${TEST_3}_disable test_3.cpp)
target_link_libraries(${TEST_3}_disable LINK_PRIVATE ${TEST_1})