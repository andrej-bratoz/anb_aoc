project("day-11")
cmake_minimum_required( VERSION 3.15)
add_executable("day_11" "main.cpp")
add_custom_command(
        TARGET day_11 POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
                ${CMAKE_SOURCE_DIR}/input_day11.txt
                ${CMAKE_CURRENT_BINARY_DIR}/input_day11.txt)
set_property(TARGET day_11 PROPERTY CXX_STANDARD 11)

