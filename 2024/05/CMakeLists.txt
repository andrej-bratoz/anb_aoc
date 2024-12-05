project("day-05")
cmake_minimum_required( VERSION 3.15)
add_executable("day_05" "main.cpp")
add_custom_command(
        TARGET day_05 POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
                ${CMAKE_SOURCE_DIR}/input_day5.txt
                ${CMAKE_CURRENT_BINARY_DIR}/input_day5.txt)

set_property(TARGET day_05 PROPERTY CXX_STANDARD 11)

