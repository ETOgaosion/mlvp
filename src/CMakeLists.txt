add_subdirectory(Config)
add_subdirectory(Library)
add_subdirectory(Database)
add_subdirectory(Transaction)
add_subdirectory(Channel)
add_subdirectory(Drivers)
add_subdirectory(Simulator)
add_subdirectory(Evaluator)
add_subdirectory(Reporter)
add_subdirectory(Spreader)
add_subdirectory(TestGenerator)

aux_source_directory(. SRC)

add_executable(mvm 
$<TARGET_OBJECTS:Config>
$<TARGET_OBJECTS:Database>
$<TARGET_OBJECTS:Transaction>
$<TARGET_OBJECTS:Drivers>
$<TARGET_OBJECTS:Simulator>
$<TARGET_OBJECTS:Evaluator>
# $<TARGET_OBJECTS:Library>
$<TARGET_OBJECTS:Reporter>
$<TARGET_OBJECTS:Spreader>
$<TARGET_OBJECTS:TestGenerator>
${SRC}
)

add_library(mvm-lib SHARED 
$<TARGET_OBJECTS:Config>
$<TARGET_OBJECTS:Database>
$<TARGET_OBJECTS:Transaction>
$<TARGET_OBJECTS:Drivers>
$<TARGET_OBJECTS:Simulator>
$<TARGET_OBJECTS:Evaluator>
# $<TARGET_OBJECTS:Library>
$<TARGET_OBJECTS:Reporter>
$<TARGET_OBJECTS:Spreader>
$<TARGET_OBJECTS:TestGenerator>
)

if (${CMAKE_BUILD_TYPE} STREQUAL "Debug")
    verilate(mvm SOURCES ${PROJECT_SOURCE_DIR}/design/NutshellCache/nutshellcache.v TOP_MODULE Cache COVERAGE OPT_SLOW TRACE)
else ()
    verilate(mvm SOURCES ${PROJECT_SOURCE_DIR}/design/NutshellCache/nutshellcache.v TOP_MODULE Cache COVERAGE OPT_FAST TRACE)
endif ()