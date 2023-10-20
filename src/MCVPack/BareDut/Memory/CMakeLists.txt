aux_source_directory(. SRC)
add_library(MemoryDut OBJECT ${SRC})
target_link_libraries(MemoryDut Vmemory__ALL.a)