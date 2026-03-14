set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --coverage")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} --coverage")

cmake_minimum_required(VERSION 3.10)
project(C++PipeLine VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(spdlog REQUIRED)
find_package(GTest REQUIRED)

add_executable(app 
    src/parsers/memParser.cpp
    src/main.cpp
    src/dispatch.cpp
    src/utility/myUnix.cpp
    src/utility/sockets.cpp
    src/parsers/verParser.cpp
)

target_include_directories(app PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(app PRIVATE spdlog::spdlog)

enable_testing()

add_executable(unit_tests 
    tests/testMemParser.cpp 
    tests/testVerParser.cpp
    tests/testMySocket.cpp
    tests/testMyUnix.cpp
    tests/testDispatch.cpp

    src/parsers/memParser.cpp
    src/dispatch.cpp
    src/utility/myUnix.cpp
    src/utility/sockets.cpp
    src/parsers/verParser.cpp
)

target_include_directories(unit_tests PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)

target_link_libraries(unit_tests PRIVATE 
    GTest::gtest_main 
    spdlog::spdlog
)

add_test(NAME MyMemTests COMMAND unit_tests)
