project("day-10")
cmake_minimum_required( VERSION 3.15)
add_executable("day_10" "main.cpp")
add_custom_command(
        TARGET day_10 POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
                ${CMAKE_SOURCE_DIR}/input_day10.txt
                ${CMAKE_CURRENT_BINARY_DIR}/input_day10.txt)
set_property(TARGET day_10 PROPERTY CXX_STANDARD 11)

