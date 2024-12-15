
        project("day-15")
        cmake_minimum_required( VERSION 3.15)
        add_executable("day_15" "main.cpp")
        add_custom_command(
                TARGET day_15 POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy
                        ${CMAKE_SOURCE_DIR}/input_day15.txt
                        ${CMAKE_CURRENT_BINARY_DIR}/input_day15.txt)
        set_property(TARGET day_15 PROPERTY CXX_STANDARD 20)
        