cmake_minimum_required(VERSION 3.16)
project(LR3 CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_CLANG_TIDY clang-tidy)

option(ENABLE_COVERAGE "Сборка с измерением покрытия (gcov)" ON)
option(USE_CLANG_TIDY OFF)
if(USE_CLANG_TIDY)
    set(CMAKE_CXX_CLANG_TIDY clang-tidy)
endif()

add_library(structures STATIC
    src/Array.cpp
    src/LinkedList.cpp
    src/DoubleLinkedList.cpp
    src/Stack.cpp
    src/Queue.cpp
    src/avlTree.cpp
    src/ChainHash.cpp
    src/CuckooHash.cpp
)
target_include_directories(structures PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/inc)
if(ENABLE_COVERAGE)
    target_compile_options(structures PUBLIC --coverage -O0 -g)
    target_link_options(structures PUBLIC --coverage)
endif()
target_compile_options(structures PRIVATE -Wall -Wextra -Wpedantic)


add_executable(structorium src/main.cpp)
target_link_libraries(structorium PRIVATE structures)

enable_testing()


find_package(Boost REQUIRED COMPONENTS unit_test_framework)
add_executable(boost_tests
    tests/test_main.cpp
    tests/test_array.cpp
    tests/test_avlTree.cpp
    tests/test_chainHash.cpp
    tests/test_cuckooHash.cpp
    tests/test_list.cpp
    tests/test_doubleLinkList.cpp
    tests/test_queue.cpp
    tests/test_stack.cpp
    tests/test_serialization.cpp
)
target_link_libraries(boost_tests PRIVATE structures Boost::unit_test_framework)
add_test(NAME boost_tests COMMAND boost_tests)



find_package(GTest REQUIRED)
include(GoogleTest)
add_executable(gtest_tests
    tests_google/test_array.cpp
    tests_google/test_avlTree.cpp
    tests_google/test_chainHash.cpp
    tests_google/test_cuckooHash.cpp
    tests_google/test_doubleLinkList.cpp
    tests_google/test_list.cpp
    tests_google/test_queue.cpp
    tests_google/test_stack.cpp
)
target_link_libraries(gtest_tests PRIVATE structures GTest::gtest GTest::gtest_main)
gtest_discover_tests(gtest_tests PROPERTIES TIMEOUT 10)



find_package(Catch2 3 REQUIRED)
include(Catch)
add_executable(catch2_tests
    tests_catch2/test_array.cpp
    tests_catch2/test_avlTree.cpp
    tests_catch2/test_chainHash.cpp
    tests_catch2/test_cuckooHash.cpp
    tests_catch2/test_doubleLinkList.cpp
    tests_catch2/test_list.cpp
    tests_catch2/test_queue.cpp
    tests_catch2/test_stack.cpp
)
target_link_libraries(catch2_tests PRIVATE structures Catch2::Catch2WithMain)
catch_discover_tests(catch2_tests PROPERTIES TIMEOUT 10)



find_package(benchmark REQUIRED)

add_executable(benchmarks 
    benchmarks/benchmarks.cpp)
target_link_libraries(benchmarks PRIVATE structures benchmark::benchmark)

