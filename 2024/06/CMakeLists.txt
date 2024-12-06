project("day-06")
cmake_minimum_required( VERSION 3.15)
add_executable("day_06" "main.cpp")
add_custom_command(
        TARGET day_06 POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
                ${CMAKE_SOURCE_DIR}/input_day6.txt
                ${CMAKE_CURRENT_BINARY_DIR}/input_day6.txt)

set_property(TARGET day_06 PROPERTY CXX_STANDARD 11)

