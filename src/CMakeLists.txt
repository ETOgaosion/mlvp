add_subdirectory(Config)
add_subdirectory(Library)
add_subdirectory(Database)
add_subdirectory(Sequencer)
add_subdirectory(Transaction)
add_subdirectory(MCVPack)
add_subdirectory(RefPack)
add_subdirectory(Driver)
add_subdirectory(Reporter)
add_subdirectory(Spreader)

add_executable(mvm 
$<TARGET_OBJECTS:Config>
$<TARGET_OBJECTS:Database>
$<TARGET_OBJECTS:Driver>
# $<TARGET_OBJECTS:Library>
$<TARGET_OBJECTS:MCVPack>
$<TARGET_OBJECTS:RefPack>
$<TARGET_OBJECTS:Reporter>
$<TARGET_OBJECTS:Spreader>
$<TARGET_OBJECTS:Sequencer>
$<TARGET_OBJECTS:Transaction>
$<TARGET_OBJECTS:MemoryDut>
$<TARGET_OBJECTS:MemoryRef>
main.cpp
)

if (${CMAKE_BUILD_TYPE} STREQUAL "Debug")
    verilate(mvm SOURCES ${PROJECT_SOURCE_DIR}/src/MCVPack/BareDut/Memory/memory.sv TOP_MODULE memory COVERAGE OPT_SLOW TRACE)
    # verilate(mvm SOURCES ${PROJECT_SOURCE_DIR}/src/MCVPack/BareDut/Mux/mux.v TOP_MODULE memory COVERAGE OPT_SLOW TRACE)
else ()
    verilate(mvm SOURCES ${PROJECT_SOURCE_DIR}/src/MCVPack/BareDut/Memory/memory.sv TOP_MODULE memory COVERAGE OPT_FAST TRACE)
    # verilate(mvm SOURCES ${PROJECT_SOURCE_DIR}/src/MCVPack/BareDut/Mux/mux.v TOP_MODULE memory COVERAGE OPT_FAST TRACE)
endif ()