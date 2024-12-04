#include <fstream>
#include <iostream>
#include <sstream>
#include <regex>
#include <vector>
#include <math.h>

struct word_matrix {
	void push_line(const std::string& s) { _lines.push_back(s); }
	char at(int row, int col) {
		return _lines[row][col];
	}
	int find_xmas_left_right() {
		int total = 0;
		for (const auto& line : _lines) {
			total += find_xmas(line);
		}
		return total;
	}
	int find_xmas_up_down() {
		int total = 0;
		for (int i = 0; i < _lines[0].length(); i++) {
			std::stringstream ss;
			for (const auto& line : _lines) {
				ss << line[i];
			}
			total += find_xmas(ss.str());
		}
		return total;
	}
	int right_diagonal() {
		int total = 0;
		int col_base = 0;
		for (int row = 0; row <= _lines.size(); row = std::min(row + 1, (int)_lines.size())) {
			std::stringstream ss;
			int rc = row;
			if (row == _lines.size()) { col_base++; rc = row - 1; }
			for (int col = col_base; col <= row; col++) {
				ss << at(rc, col);
				rc--;
			}
			if (ss.str().length() == 0) break;
			total += find_xmas(ss.str());
		}
		return total;
	}
	int left_diagonal() {
		int total = 0;
		int col_reach = 0;
		int col_start = 0;
		for (int row = _lines.size() - 1; row >= 0; row = std::max(row - 1, 0)) {
			std::stringstream ss;
			int rc = row;
			for (int col = col_start; col <= col_reach; col++) {
				ss << at(rc, col);
				rc++;
			}
			if (row == 0) {
				col_start++;
			}
			col_reach = std::min(col_reach + 1, (int)_lines[0].length());
			total += find_xmas(ss.str());
			if (col_start == _lines[0].length()) break;
		}
		return total;
	}
	//
	word_matrix rotate90() {
		word_matrix wm;
		for (int i = 0; i < _lines[0].length(); i++) {
			std::stringstream ss;
			for (const auto& line : _lines) {
				ss << line[i];
			}
			wm.push_line(ss.str());
		}
		return wm;
	}
	word_matrix get_3x3_at(int row, int col) {
		word_matrix wm;
		std::stringstream ss;
		ss << at(row, col) << at(row, col + 1) << at(row, col + 2);
		wm.push_line(ss.str());
		ss.str("");
		ss.clear();
		ss << at(row + 1, col) << at(row + 1, col + 1) << at(row +1, col + 2);
		wm.push_line(ss.str());
		ss.str("");
		ss.clear();
		ss << at(row + 2, col) << at(row + 2, col + 1) << at(row + 2, col + 2);
		wm.push_line(ss.str());
		return wm;
	}
	int find_x_max() {
		int total = 0;
		for (int row = 0; row <= _lines.size() - 3; row++) {
			for (int col = 0; col <= _lines[0].length() - 3; col++) {
				auto mx = get_3x3_at(row, col);
				if (mx.is_x_max()) {
					total++;
				}
			}
		}
		return total;
	}

	private:
	int find_xmas(std::string line) {
		int finds = 0;
		if (line.length() < 4) return 0;
		if (line.length() == 4 && (line == "XMAS" || line == "SAMX")) return 1;
		for (int i = 0; i <= line.length() - 4; i++) {
			auto nl = line.substr(i, 4);
			if (nl == "XMAS" || nl == "SAMX") finds++;
		}
		return finds;
	}
	bool is_x_max() {
		bool is_xm = false;
		{
			{
				std::smatch match1, match2, match3;
				std::regex_match(_lines[0], match1, std::regex("M.S"));
				std::regex_match(_lines[1], match2, std::regex(".A."));
				std::regex_match(_lines[2], match3, std::regex("M.S"));
				is_xm = is_xm || (!match1.empty() && !match2.empty() && !match3.empty());
			}
			{
				std::smatch match1, match2, match3;
				std::regex_match(_lines[0], match1, std::regex("S.M"));
				std::regex_match(_lines[1], match2, std::regex(".A."));
				std::regex_match(_lines[2], match3, std::regex("S.M"));
				is_xm = is_xm || (!match1.empty() && !match2.empty() && !match3.empty());
			}
		}
		return is_xm;
	}

	std::vector<std::string> _lines;
};

void get_word_matrix(const std::string& file, word_matrix& wm) {
	std::ifstream ifs(file);
	if (!ifs.is_open()) { return; }
	std::string line;
	while (std::getline(ifs, line)) {
		wm.push_line(line);
	}
}

void write_sln(const std::string& name, const std::string& sln) {
	std::ofstream ofs(name, std::ios::trunc);
	ofs << sln;
}

void part_1() {
	int total_sum = 0;
	word_matrix wm;
	get_word_matrix("input_day4.txt", wm);
	total_sum += wm.find_xmas_left_right();
	total_sum += wm.find_xmas_up_down();
	total_sum += wm.right_diagonal();
	total_sum += wm.left_diagonal();
	write_sln("day4_sln_p1.txt", std::to_string(total_sum));
}

void part_2() {
	int total_sum = 0;
	word_matrix wm;
	get_word_matrix("input_day4.txt", wm);
	total_sum = wm.find_x_max();
	total_sum += wm.rotate90().find_x_max();
	write_sln("day4_sln_p2.txt", std::to_string(total_sum));
}

int main() {
	part_1();
	part_2();
	return 0;
}