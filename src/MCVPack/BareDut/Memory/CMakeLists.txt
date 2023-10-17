aux_source_directory(. SRC)
add_library(dut OBJECT ${SRC})
target_link_libraries(dut Vmemory__ALL.a)