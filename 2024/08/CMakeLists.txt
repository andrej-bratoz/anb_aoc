project("day-08")
cmake_minimum_required( VERSION 3.15)
add_executable("day_08" "main.cpp")
add_custom_command(
        TARGET day_08 POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
                ${CMAKE_SOURCE_DIR}/input_day8.txt
                ${CMAKE_CURRENT_BINARY_DIR}/input_day8.txt)
set_property(TARGET day_08 PROPERTY CXX_STANDARD 11)

