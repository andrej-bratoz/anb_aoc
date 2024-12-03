project("day-03")
cmake_minimum_required( VERSION 3.15)
add_executable("day_03" "main.cpp")
add_custom_command(
        TARGET day_03 POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
                ${CMAKE_SOURCE_DIR}/input_day3.txt
                ${CMAKE_CURRENT_BINARY_DIR}/input_day3.txt)

set_property(TARGET day_03 PROPERTY CXX_STANDARD 11)

