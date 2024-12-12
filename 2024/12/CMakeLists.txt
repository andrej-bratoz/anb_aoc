
        project("day-12")
        cmake_minimum_required( VERSION 3.15)
        add_executable("day_12" "main.cpp")
        add_custom_command(
                TARGET day_12 POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy
                        ${CMAKE_SOURCE_DIR}/input_day12.txt
                        ${CMAKE_CURRENT_BINARY_DIR}/input_day12.txt)
        set_property(TARGET day_12 PROPERTY CXX_STANDARD 20)
        