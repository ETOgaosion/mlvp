# cmake for mvm

cmake_minimum_required(VERSION 3.16.3)
project(MVM VERSION 0.0.1)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_VERBOSE_MAKEFILE OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)
set(CMAKE_VERBOSE_MAKEFILE ON)
add_definitions(-DCMAKE_EXPORT_COMPILE_COMMANDS=ON)

set(CMAKE_BUILD_TYPE "Debug")
# set(CMAKE_BUILD_TYPE "Release")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC")

set(VERILATOR_ACTUAL_ROOT $ENV{VERILATOR_ROOT})

if("${VERILATOR_ACTUAL_ROOT}" STREQUAL "")
    set(VERILATOR_ACTUAL_ROOT "$ENV{HOME}/softwares/verilator")
endif()

find_package(verilator HINTS ${VERILATOR_ACTUAL_ROOT})

set(VERILATOR_INCLUDE_DIRS ${VERILATOR_ACTUAL_ROOT}/include)
include_directories(${VERILATOR_INCLUDE_DIRS})

include_directories(include)
include_directories(third-party/include)

add_definitions( -DUSE_THREADS=false )
add_definitions( -DBUGDEGREE=Degree::LOW )

add_subdirectory(src)
add_subdirectory(tools)
add_subdirectory(tests)

# install
set(CMAKE_INSTALL_PREFIX /usr/local)
set(CMAKE_INSTALL_LOCAL_ONLY TRUE)

install(
    TARGETS mvm-lib 
    LIBRARY DESTINATION lib/
)
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