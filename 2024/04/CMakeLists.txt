project("day-04")
cmake_minimum_required( VERSION 3.15)
add_executable("day_04" "main.cpp")
add_custom_command(
        TARGET day_04 POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
                ${CMAKE_SOURCE_DIR}/input_day4.txt
                ${CMAKE_CURRENT_BINARY_DIR}/input_day4.txt)

set_property(TARGET day_04 PROPERTY CXX_STANDARD 11)

