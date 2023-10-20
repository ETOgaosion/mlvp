# add_subdirectory(Mux)
add_subdirectory(Memory)

add_library(BareDut OBJECT $<TARGET_OBJECTS:MemoryDut>)