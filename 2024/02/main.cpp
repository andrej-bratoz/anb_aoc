#include <fstream>
#include <iostream>
#include <string>
#include <regex>
#include <vector>

struct report {
	void add(int level) { _levels.push_back(level); }
	std::vector<int> _levels;

	report copy_without_index(const report& r, int idx) {
		report rep;
		for (int i = 0; i < r._levels.size(); i++) {
			if (i != idx) {
				rep.add(r._levels[i]);
			}
		}
		return rep;
	}

	bool is_safe() {
		if (!_init && _levels.size() >= 2) {
			// Special case, not increasing or decreasing, cannot get tendency
			if (_levels[0] == _levels[1]) {
				return false;
			}
			//
			if (_levels[0] < _levels[1]) {
				_increasing = true;
			}
			else {
				_increasing = false;
			}
			_init = true;
		}
		int prev = _levels[0];
		for (int i = 1; i < _levels.size(); i++) {
			int current = _levels[i];
			if (_increasing && current <= prev) return false;
			if (!_increasing && prev <= current) return false;

			int diff = std::abs(prev - current);
			if (diff < 1 || diff > 3) return false;

			prev = current;

		}
		return true;
	}

	bool is_safe_dampened() {
		if (is_safe()) return true;
		// Dumb brute force approach but I am tired
		bool is_safe_currently = false;
		for (int i = 0; i < _levels.size(); i++) {
			auto cpy = copy_without_index(*this, i);
			if (cpy.is_safe()) return true;
		}
		return false;
	}
	bool _init = false;
	bool _increasing = false;
};


void get_vectors(std::string file, std::vector<report>& reports) {
	std::ifstream ifs(file);
	if (!ifs.is_open()) { return; }

	std::string line;
	while (std::getline(ifs, line)) {
		std::regex r(R"(\d+)");
		report report;
		for (std::smatch sm; std::regex_search(line, sm, r);) {
			auto number = sm.str();
			report.add(std::atoi(number.c_str()));
			line = sm.suffix();
		}
		reports.push_back(report);
	}
}

void write_sln(const std::string& name, const std::string& sln) {
	std::ofstream ofs(name, std::ios::trunc);
	ofs << sln;
}

void part_1() {
	std::vector<report> reports;
	get_vectors(R"(input_day2.txt)", reports);
	int total_sum = 0;
	for (auto& report : reports) {
		if(report.is_safe()) {
			total_sum++;
		}
	}
	write_sln("day2_sln_p1.txt", std::to_string(total_sum));
}

void part_2() {
	std::vector<report> reports;
	get_vectors(R"(input_day2.txt)", reports);
	int total_sum = 0;
	for (auto& report : reports) {
		if (report.is_safe_dampened()) {
			total_sum++;
		}
	}
	write_sln("day2_sln_p2.txt", std::to_string(total_sum));
}

int main() {
	part_1();
	part_2();
	return 0;
}