project("day-01")
cmake_minimum_required( VERSION 3.15)
add_executable("day_01" "main.cpp")
add_custom_command(
        TARGET day_01 POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
                ${CMAKE_SOURCE_DIR}/input_day1.txt
                ${CMAKE_CURRENT_BINARY_DIR}/input_day1.txt)

set_property(TARGET day_01 PROPERTY CXX_STANDARD 11)

