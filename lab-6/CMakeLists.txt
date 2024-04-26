cmake_minimum_required(VERSION 3.12)
project(LAB6)

set(CMAKE_CXX_STANDARD 20)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

FIND_PACKAGE( Boost 1.74 COMPONENTS program_options REQUIRED )
INCLUDE_DIRECTORIES( ${Boost_INCLUDE_DIR} )

add_executable(main main.cpp)
target_compile_options(main PUBLIC -std=c++20 -Wall -Wextra)
target_link_libraries(main PUBLIC boost_regex)
