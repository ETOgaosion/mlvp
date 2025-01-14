# cmake for mlvp

cmake_minimum_required(VERSION 3.16.3)
project(MLVP VERSION 0.0.1)

if (CMAKE_CXX_COMPILER_VERSION VERSION_LESS 10.0)
    message(FATAL_ERROR "GCC version must be at least 10.0!  " ${CMAKE_CXX_COMPILER_VERSION})
endif()

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_VERBOSE_MAKEFILE OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)
set(CMAKE_VERBOSE_MAKEFILE ON)
add_definitions(-DCMAKE_EXPORT_COMPILE_COMMANDS=ON)

# set(CMAKE_BUILD_TYPE "Debug")
set(CMAKE_BUILD_TYPE "Release")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC")

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -O0 -pg -finstrument-functions")
else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -O3")
endif()

set(VERILATOR_ACTUAL_ROOT $ENV{VERILATOR_ROOT})

if("${VERILATOR_ACTUAL_ROOT}" STREQUAL "")
    set(VERILATOR_ACTUAL_ROOT "$ENV{HOME}/softwares/verilator")
endif()

find_package(verilator HINTS ${VERILATOR_ACTUAL_ROOT})

set(VERILATOR_INCLUDE_DIRS ${VERILATOR_ACTUAL_ROOT}/include)
include_directories(${VERILATOR_INCLUDE_DIRS})

include_directories(include)
include_directories(third-party/include)

add_definitions( -DUSE_THREADS=true )
add_definitions( -DUSE_SYNC=false )
add_definitions( -DSHOW_TEXT_WAVE=false )
add_definitions( -DBUGDEGREE=Degree::LOW )

add_subdirectory(src)
add_subdirectory(tools)
add_subdirectory(tests)

# install
set(CMAKE_INSTALL_PREFIX /usr/local)
set(CMAKE_INSTALL_LOCAL_ONLY TRUE)

# install(
#     TARGETS mlvp-lib 
#     LIBRARY DESTINATION lib/
# )
install(
    DIRECTORY include DESTINATION include
)

# make uninstall
add_custom_target("uninstall" COMMENT "Uninstall installed files")
add_custom_command(
    TARGET "uninstall"
    POST_BUILD
    COMMENT "Uninstall files with install_manifest.txt"
    COMMAND xargs rm -vf < install_manifest.txt
)