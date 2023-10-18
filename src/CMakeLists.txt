# FILEPATH: /home/gzy/projects/mvm/src/CMakeLists.txt

# Add all subdirectories in the current directory
file(GLOB SUBDIR_LIST RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} *)

include_directories(../include)

foreach(SUBDIR ${SUBDIR_LIST})
    if(IS_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/${SUBDIR})
        add_subdirectory(${SUBDIR})
    endif()
endforeach()

add_executable(mvm main.cpp)