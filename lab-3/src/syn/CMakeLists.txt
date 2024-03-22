cmake_minimum_required(VERSION 3.12)
project(syn_server)

set(CMAKE_CXX_STANDARD 20)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
file(GLOB_RECURSE SRC "libs/*.h" "libs/*.cpp")

FIND_PACKAGE( Boost 1.72 COMPONENTS program_options REQUIRED )
INCLUDE_DIRECTORIES( ${Boost_INCLUDE_DIR} )

add_library(Libs ${SRC})
link_libraries(Libs)

file(GLOB APP_SOURCES "*.cpp")

foreach(sourcefile ${APP_SOURCES})
    string(REGEX REPLACE ".+/(.+)\\..*" "\\1" name ${sourcefile})
    add_executable(${name} ${sourcefile})
    target_include_directories(${name} PUBLIC src)
    TARGET_LINK_LIBRARIES( ${name} LINK_PUBLIC ${Boost_LIBRARIES} )
endforeach(sourcefile ${APP_SOURCES})