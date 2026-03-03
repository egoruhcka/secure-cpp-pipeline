cmake_minimum_required(VERSION 3.10)

project(C++PipeLine)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(app 
    src/MemParcer.cpp
)

target_include_directories(app PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

find_package(spdlog REQUIRED)
target_link_libraries(app PRIVATE spdlog::spdlog)

