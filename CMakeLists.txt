cmake_minimum_required(VERSION 3.10)
project(Heimdall)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(SDL2 REQUIRED)

if("${SDL2_LIBRARIES}" STREQUAL "")
    message(WARNING "SDL2_LIBRARIES wasn't set, manually setting to SDL2::SDL2")
    set(SDL2_LIBRARIES "SDL2::SDL2")
endif()

include_directories(${CMAKE_BINARY_DIR})
include_directories(src/)

include_directories(${SDL2_INCLUDE_DIRS})

add_library(heimdall STATIC
    src/heimdall/engine.h
    src/heimdall/engine.cpp
    src/heimdall/component.cpp
    src/heimdall/component.h
    src/heimdall/componentHandle.h
    src/heimdall/componentManager.h
    src/heimdall/componentMask.cpp
    src/heimdall/componentMask.h
    src/heimdall/entity.h
    src/heimdall/entityHandle.h
    src/heimdall/entityManager.cpp
    src/heimdall/entityManager.h
    src/heimdall/entityMap.h
    src/heimdall/system.cpp
    src/heimdall/system.h
    src/heimdall/world.cpp
    src/heimdall/world.h
    src/heimdall/system/log.cpp
    src/heimdall/system/log.h
    src/heimdall/system/stacktrace.cpp
    src/heimdall/system/stacktrace.h
    src/heimdall/system/h_alloc.cpp
    src/heimdall/system/h_alloc.h
)

add_executable(main
    src/main.cpp
)

target_link_libraries(main ${SDL2_LIBRARIES} heimdall)
