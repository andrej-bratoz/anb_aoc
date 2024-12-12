import sys
import os
import subprocess

day = sys.argv[1]

HEADERS = """#include <fstream>
#include <iostream>
#include <regex>
#include <vector>
#include <sstream>
#include <map>
#include <locale>"""

SPLIT="""// thank you, SO [https://stackoverflow.com/a/46931770/732348]
std::vector<std::string> split(std::string s, std::string delimiter) {
	size_t pos_start = 0, pos_end, delim_len = delimiter.length();
	std::string token;
	std::vector<std::string> res;
	while ((pos_end = s.find(delimiter, pos_start)) != std::string::npos) {
		token = s.substr(pos_start, pos_end - pos_start);
		pos_start = pos_end + delim_len;
		res.push_back(token);
	}
	res.push_back(s.substr(pos_start));
	return res;
}"""

WRITE_SLN="""void write_sln(const std::string& name, const std::string& sln) {
	std::ofstream ofs(name, std::ios::trunc);
	ofs << sln;
}"""

LOAD_DATA="""
void load_data(const std::string& file, std::vector<std::string>& lines) {
    std::ifstream ifs(file);
	if (!ifs.is_open()) { return; }
	std::string line;
	while(std::getline(ifs, line)) {
        lines.push_back(line);
    }
}"""

MAIN_CPP="""{0}\n{1}\n{2}\n{3}
void part_1() {{ 
    std::vector<std::string> lines;
    load_data("input_day{4}.txt", lines);
    write_sln("solution_day{4}_p1.txt", ""); 
}}
void part_2() {{ 
    std::vector<std::string> lines;
    load_data("input_day{4}.txt", lines);
    write_sln("solution_day{4}_p2.txt", ""); 
}}
int main() {{
    part_1();
    part_2();
    return 0;
}}
""".format(HEADERS, SPLIT, WRITE_SLN, LOAD_DATA, day)           

root = os.path.dirname(__file__)
full_path = os.path.join(root, day)

def write_file(path, value):
    text_file = open(path, "w")
    text_file.write(value)
    text_file.close()

print(full_path)
if not os.path.exists(full_path):
    os.makedirs(full_path)
    os.makedirs(os.path.join(full_path,"_sln"))
    write_file(os.path.join(full_path, "input_day{0}.txt").format(day), "")
    write_file(os.path.join(full_path, "CMakeLists.txt"),
        """
        project("day-{0}")
        cmake_minimum_required( VERSION 3.15)
        add_executable("day_{0}" "main.cpp")
        add_custom_command(
                TARGET day_{0} POST_BUILD
                COMMAND ${{CMAKE_COMMAND}} -E copy
                        ${{CMAKE_SOURCE_DIR}}/input_day{0}.txt
                        ${{CMAKE_CURRENT_BINARY_DIR}}/input_day{0}.txt)
        set_property(TARGET day_{0} PROPERTY CXX_STANDARD 20)
        """.format(day)
    )
    write_file(os.path.join(full_path, "main.cpp"), MAIN_CPP)

os.chdir(os.path.join(full_path,"_sln"))
os.system("cmake ..")