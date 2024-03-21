cmake_minimum_required(VERSION 3.12)
project(dos_server)

set(CMAKE_CXX_STANDARD 20)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
file(GLOB_RECURSE SRC "libs/*.h" "libs/*.cpp")

add_library(Libs ${SRC})
link_libraries(Libs)

file(GLOB APP_SOURCES "*.cpp")

foreach(sourcefile ${APP_SOURCES})
    string(REGEX REPLACE ".+/(.+)\\..*" "\\1" name ${sourcefile})
    add_executable(${name} ${sourcefile})
    target_include_directories(${name} PUBLIC src)
endforeach(sourcefile ${APP_SOURCES})

# add_executable(client client.cpp)
# target_compile_options(client PUBLIC -std=c++20 -Wall -Wextra)

# add_executable(server server.cpp)
# target_compile_options(server PUBLIC -std=c++20 -Wall -Wextra)