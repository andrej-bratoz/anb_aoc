#include <fstream>
#include <iostream>
#include <regex>
#include <vector>
#include <limits>
#include <sstream>

#include <algorithm> 
#include <cctype>
#include <locale>


struct matrix {
	std::vector<std::string> lines;
	std::vector<std::string> empty_mx;

	struct point {
		int x = -1;
		int y = -1;
	};

	void print_antinodes() {
		for (const auto& str : empty_mx) {
			std::cout << str << "\n";
		}
	}

	void invert_world() {
		std::reverse(lines.begin(), lines.end());
		std::reverse(empty_mx.begin(), empty_mx.end());
	}

	int calculate_all_antinodes() {
		int total = 0;
		for (const auto& line : empty_mx) {
			for (auto ch : line) {
				if (ch == '#') total++;
			}
		}
		return total;
	}

	int calculate_all_antinodes2() {
		int total = 0;
		for (int i = 0; i < lines[0].length(); i++) {
			for (int j = 0; j < lines.size(); j++) {
				if (lines[j][i] != '.' || empty_mx[j][i] == '#') total++;
			}
		}

		return total;
	}


	//
	void create_empty_matrix() {
		for(const auto& line : lines) {
			std::stringstream ss;
			for (int i = 0; i < line.length(); i++) {
				ss << '.';
			}
			empty_mx.push_back(ss.str());
		}
	}
	void calculate_antinodes_up_down() {
		for(int row_idx = 0; row_idx < lines[0].length(); row_idx++) {
			for(int col_idx = 0; col_idx < lines.size(); col_idx++) {
				if(lines[col_idx][row_idx] != '.') {
					auto same_towers = down_antinode(row_idx, col_idx, lines[col_idx][row_idx]);
					for(auto& tower : same_towers) {
						int y_diff = tower.y - col_idx;
						int x_diff = tower.x - row_idx;

						int antinode_x = tower.x + x_diff;
						int antinode_y = tower.y + y_diff;
						// outsode of grid
						if(antinode_x < 0 || antinode_x > lines[0].length() - 1) continue;
						if(antinode_y < 0 || antinode_y > lines.size() -1 ) continue;;
						empty_mx[antinode_y][antinode_x] = '#';
					}
				}
			}
		}
	}

	void calculate_antinodes_up_down2() {
		for (int row_idx = 0; row_idx < lines[0].length(); row_idx++) {
			for (int col_idx = 0; col_idx < lines.size(); col_idx++) {
				if (lines[col_idx][row_idx] != '.') {
					auto same_towers = down_antinode(row_idx, col_idx, lines[col_idx][row_idx]);
					for (auto& tower : same_towers) {
						int y_diff = tower.y - col_idx;
						int x_diff = tower.x - row_idx;
						do {
							int antinode_x = tower.x + x_diff;
							int antinode_y = tower.y + y_diff;
							// outsode of grid
							if (antinode_x < 0 || antinode_x > lines[0].length() - 1) break;
							if (antinode_y < 0 || antinode_y > lines.size() - 1) break;
							tower.x = antinode_x;
							tower.y = antinode_y;
							empty_mx[antinode_y][antinode_x] = '#';
						} while (true);
					}
				}
			}
		}
	}

	std::vector<point> down_antinode(int in_x, int in_y, char ch) {
		std::vector<point> res;
		for(int row_inx = 0; row_inx < lines[0].length(); row_inx++) {
			for(int col_idx = in_y + 1; col_idx < lines.size(); col_idx++) {
				if(lines[col_idx][row_inx] == ch) {
					res.push_back({row_inx, col_idx});
				}
			}
		}
		return res;
	}
};


void load_data(const std::string& file, std::vector<std::string>& lines) {
	std::ifstream ifs(file);
	if (!ifs.is_open()) { return; }
	std::string arith_line;
	int row = 0;
	while (std::getline(ifs, arith_line)) {
		lines.push_back(arith_line);
	}
}

void write_sln(const std::string& name, const std::string& sln) {
	std::ofstream ofs(name, std::ios::trunc);
	ofs << sln;
}

void part_1() {
	unsigned long long target = 0;
	std::vector<std::string> lines;
	load_data("input_day8.txt", lines);
	matrix mx;
	mx.lines = lines;
	mx.create_empty_matrix();
	mx.calculate_antinodes_up_down();
	mx.invert_world();
	mx.calculate_antinodes_up_down();
	target = mx.calculate_all_antinodes();
	write_sln("solution_day8_p1.txt", std::to_string(target));
}

void part_2() {
	unsigned long long target = 0;
	std::vector<std::string> lines;
	load_data("input_day8.txt", lines);
	matrix mx;
	mx.lines = lines;
	mx.create_empty_matrix();
	mx.calculate_antinodes_up_down2();
	mx.invert_world();
	mx.calculate_antinodes_up_down2();
	target = mx.calculate_all_antinodes2();
	write_sln("solution_day8_p2.txt", std::to_string(target));
}

int main() {
	part_1();
	part_2();
	return 0;
}
