add_subdirectory(cpp)

add_executable(TestGenerator
    $<TARGET_OBJECTS:GeneratorHelper>
    main.cpp
)