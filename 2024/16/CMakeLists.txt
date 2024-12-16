
        project("day-16")
        cmake_minimum_required( VERSION 3.15)
        add_executable("day_16" "main.cpp")
        add_custom_command(
                TARGET day_16 POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy
                        ${CMAKE_SOURCE_DIR}/input_day16.txt
                        ${CMAKE_CURRENT_BINARY_DIR}/input_day16.txt)
        set_property(TARGET day_16 PROPERTY CXX_STANDARD 20)
        