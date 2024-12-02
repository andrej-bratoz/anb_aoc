project("day-02")
cmake_minimum_required( VERSION 3.15)
add_executable("day_02" "main.cpp")
add_custom_command(
        TARGET day_02 POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
                ${CMAKE_SOURCE_DIR}/input_day2.txt
                ${CMAKE_CURRENT_BINARY_DIR}/input_day2.txt)

set_property(TARGET day_02 PROPERTY CXX_STANDARD 11)

