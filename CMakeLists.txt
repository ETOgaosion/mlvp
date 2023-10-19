# cmake for mvm

cmake_minimum_required(VERSION 3.16.3)
project(MVM VERSION 0.0.1)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_VERBOSE_MAKEFILE OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)
add_definitions(-DCMAKE_EXPORT_COMPILE_COMMANDS=ON)

set(CMAKE_BUILD_TYPE "Debug")
# set(CMAKE_BUILD_TYPE "Release")

set(VERILATOR_ACTUAL_ROOT $ENV{VERILATOR_ROOT})

if("${VERILATOR_ACTUAL_ROOT}" STREQUAL "")
    set(VERILATOR_ACTUAL_ROOT "$ENV{HOME}/softwares/verilator")
endif()

find_package(verilator HINTS ${VERILATOR_ACTUAL_ROOT})

set(VERILATOR_INCLUDE_DIRS ${VERILATOR_ACTUAL_ROOT}/include)
include_directories(${VERILATOR_INCLUDE_DIRS})

add_subdirectory(src)
add_subdirectory(tools)
add_subdirectory(tests)