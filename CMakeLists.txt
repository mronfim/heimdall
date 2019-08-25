cmake_minimum_required(VERSION 3.10)
project(Heimdall)

find_package(SDL2 REQUIRED)

if("${SDL2_LIBRARIES}" STREQUAL "")
    message(WARNING "SDL2_LIBRARIES wasn't set, manually setting to SDL2::SDL2")
    set(SDL2_LIBRARIES "SDL2::SDL2")
endif()

include_directories(${CMAKE_BINARY_DIR})
include_directories(src/)

include_directories(${SDL2_INCLUDE_DIRS})

add_library(system STATIC
    src/system/log.c
    src/system/log.h
    src/system/lt.h
    src/system/stacktrace.c
    src/system/stacktrace.h
)

add_executable(heimdall
    src/main.c
)

target_link_libraries(heimdall ${SDL2_LIBRARIES} system)