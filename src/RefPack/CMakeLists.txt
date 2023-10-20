add_subdirectory(Memory)
# add_subdirectory(Mux)

add_library(RefPack OBJECT
    $<TARGET_OBJECTS:MemoryRef>
    refDriver.cpp
)