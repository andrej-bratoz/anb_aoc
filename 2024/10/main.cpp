#include <fstream>
#include <iostream>
#include <regex>
#include <vector>
#include <limits>
#include <sstream>

#include <algorithm> 
#include <cctype>
#include <locale>

#include <set>



struct matrix {
	struct point;
	std::vector<std::vector<int>> lines;
	std::vector<point> starting_points;
	matrix(const std::vector<std::vector<int>>& nums) : lines(nums) {}

	struct point {
		int x = -1;
		int y = -1;
		int val = -1;
		point() {}
		point(int _x, int _y) : x(_x), y(_y) {}
		bool operator==(const point& pt) const {
			return pt.x == x && pt.y == y;
		}
	};

	int at(const point& p) {
		if (p.y < 0 || p.x < 0) return -1;
		if (p.y >= lines.size()) return -1;
		if (p.x >= lines[0].size()) return -1;
		return lines[p.y][p.x];
	}

	bool contains(const std::vector<point>& path, const point& p) {
		return std::count(path.begin(), path.end(), p) > 0;
	}


	int find_number_of_all_paths() {
		int num_paths = 0;
		int toals = 0;
		starting_points.clear();
		find_all_starting_points();
		std::vector<std::vector<point>> paths;
		int totals = 0;
		for (const auto& p : starting_points) {
			std::vector<point> path;
			std::vector<point> final_points;
			path.push_back(p);
			path_exists_from_point(0, p, path , paths, num_paths);
		}
		return num_paths;
	}

	
	int find_number_of_unique_paths() {
		int num_paths = 0;
		int toals = 0;
		starting_points.clear();
		find_all_starting_points();
		std::vector<std::vector<point>> paths;
		int totals = 0;
		
		for (const auto& p : starting_points) {
			std::vector<point> path;
			std::vector<point> final_points;
			path.push_back(p);
			path_exists_from_point(0, p, path, paths, num_paths);
			std::vector<point> unique_end_points;
			for (const auto& pp : paths) {
				auto ppppp = pp[pp.size() - 1];
				ppppp.val = at(ppppp);
				if (!contains(unique_end_points, ppppp) && pp[0] == p) {
					unique_end_points.push_back(ppppp);
				}
			}
			toals += unique_end_points.size();
		}
		return toals;
	}

	

	void find_all_starting_points() {
		for (int col_idx = 0; col_idx < lines.size(); col_idx++) {
			for (int row_idx = 0; row_idx < lines[0].size(); row_idx++) {
				if (lines[col_idx][row_idx] == 0) {
					starting_points.push_back({ row_idx, col_idx });
				}
			}
		}
	}

	

	void path_exists_from_point(int current_num, const point& current_point, std::vector<point>& path, std::vector<std::vector<point>>& full_path, int& finished_paths) {


		if (path.size() == 10 && at(path[path.size() - 1]) == 9) {
			finished_paths++;
			full_path.push_back(path);
			return;
		}

		if (current_point.x < 0 || current_point.x >= lines[0].size()) {
			path.clear();
			return;
		}
		if (current_point.y < 0 || current_point.y >= lines.size()) {
			path.clear();
			return;
		}

		point next_point_left(current_point.x - 1, current_point.y);
		point next_point_above(current_point.x, current_point.y - 1);
		point next_point_right(current_point.x + 1, current_point.y);
		point next_point_bottom(current_point.x, current_point.y + 1);

		if (at(next_point_left) == at(current_point) + 1) {
			std::vector<point> pth(path.begin(), path.end());
			pth.push_back(next_point_left);
			path_exists_from_point(at(next_point_left), next_point_left, pth, full_path, finished_paths);
		}

		if (at(next_point_above) == at(current_point) + 1) {
			std::vector<point> pth(path.begin(), path.end());
			pth.push_back(next_point_above);
			path_exists_from_point(at(next_point_above), next_point_above, pth, full_path, finished_paths);
		}
		
		if (at(next_point_right) == at(current_point) + 1) {
			std::vector<point> pth(path.begin(), path.end());
			pth.push_back(next_point_right);
			path_exists_from_point(at(next_point_right), next_point_right, pth, full_path, finished_paths);
		}

		if (at(next_point_bottom) == at(current_point) + 1) {
			std::vector<point> pth(path.begin(), path.end());
			pth.push_back(next_point_bottom);
			path_exists_from_point(at(next_point_bottom), next_point_bottom, pth, full_path, finished_paths);
		}
	}

};


void load_data(const std::string& file, std::vector<std::vector<int>>& lines) {
	std::ifstream ifs(file);
	if (!ifs.is_open()) { return; }
	std::string arith_line;
	int row = 0;
	while (std::getline(ifs, arith_line)) {
		std::vector<int> line;
		for (auto ch : arith_line) {
			if (ch == '.') line.push_back(INT_MAX);
			else line.push_back(ch - '0');
		}
		lines.push_back(line);
	}
}

void write_sln(const std::string& name, const std::string& sln) {
	std::ofstream ofs(name, std::ios::trunc);
	ofs << sln;
}

void part_1() {
	unsigned long long target = 0;
	std::vector<std::vector<int>> lines;
	load_data("input_day10.txt", lines);
	matrix m(lines);
	target = m.find_number_of_unique_paths();
	write_sln("solution_day10_p1.txt", std::to_string(target));
}

void part_2() {
	unsigned long long target = 0;
	std::vector<std::vector<int>> lines;
	load_data("input_day10.txt", lines);
	matrix m(lines);
	target = m.find_number_of_all_paths();
	write_sln("solution_day10_p2.txt", std::to_string(target));
}

int main() {
	part_1();
	part_2();
	return 0;
}
