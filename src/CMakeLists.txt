add_subdirectory(Config)
add_subdirectory(Library)
add_subdirectory(Database)
add_subdirectory(TestGenerator)
add_subdirectory(Sequencer)
add_subdirectory(Transaction)
add_subdirectory(Driver)
add_subdirectory(Reporter)
add_subdirectory(Spreader)

aux_source_directory(. SRC)

add_executable(mvm 
$<TARGET_OBJECTS:Config>
$<TARGET_OBJECTS:Database>
$<TARGET_OBJECTS:Driver>
# $<TARGET_OBJECTS:Library>
$<TARGET_OBJECTS:Reporter>
$<TARGET_OBJECTS:Spreader>
$<TARGET_OBJECTS:Sequencer>
$<TARGET_OBJECTS:TestGenerator>
$<TARGET_OBJECTS:Transaction>
${SRC}
)

add_library(mvm-lib SHARED 
$<TARGET_OBJECTS:Config>
$<TARGET_OBJECTS:Database>
$<TARGET_OBJECTS:Driver>
# $<TARGET_OBJECTS:Library>
$<TARGET_OBJECTS:Reporter>
$<TARGET_OBJECTS:Spreader>
$<TARGET_OBJECTS:Sequencer>
$<TARGET_OBJECTS:TestGenerator>
$<TARGET_OBJECTS:Transaction>
)

if (${CMAKE_BUILD_TYPE} STREQUAL "Debug")
    verilate(mvm SOURCES ${PROJECT_SOURCE_DIR}/design/Memory/memory.sv TOP_MODULE memory COVERAGE OPT_SLOW TRACE)
else ()
    verilate(mvm SOURCES ${PROJECT_SOURCE_DIR}/design/Memory/memory.sv TOP_MODULE memory COVERAGE OPT_FAST TRACE)
endif ()