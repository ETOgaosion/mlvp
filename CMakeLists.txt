# cmake for mvm

cmake_minimum_required(VERSION 3.11)
project(mcv VERSION 0.0.1)

set(CMAKE_CXX_STANDARD 17)

if(CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_SOURCE_DIR)
    set(is_top_level_project TRUE)
else()
    set(is_top_level_project FALSE)
endif()

# ...