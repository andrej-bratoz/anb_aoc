project("day-07")
cmake_minimum_required( VERSION 3.15)
add_executable("day_07" "main.cpp")
add_custom_command(
        TARGET day_07 POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
                ${CMAKE_SOURCE_DIR}/input_day7.txt
                ${CMAKE_CURRENT_BINARY_DIR}/input_day7.txt)
set_property(TARGET day_07 PROPERTY CXX_STANDARD 11)

