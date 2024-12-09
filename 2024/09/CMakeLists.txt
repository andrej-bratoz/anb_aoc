project("day-09")
cmake_minimum_required( VERSION 3.15)
add_executable("day_09" "main.cpp")
add_custom_command(
        TARGET day_09 POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
                ${CMAKE_SOURCE_DIR}/input_day9.txt
                ${CMAKE_CURRENT_BINARY_DIR}/input_day9.txt)
set_property(TARGET day_09 PROPERTY CXX_STANDARD 11)

