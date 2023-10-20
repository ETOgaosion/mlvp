add_subdirectory(BareDut)

add_library(MCVPack OBJECT
    $<TARGET_OBJECTS:BareDut>
    dutDriver.cpp
)