#include <fstream>
#include <iostream>
#include <string>
#include <regex>
#include <vector>

void get_vectors(std::string file, std::vector<int>& left, std::vector<int>& right) {
	std::ifstream ifs(file);
	if (!ifs.is_open()) { return; }

	std::string line;
	while (std::getline(ifs, line)) {
		std::smatch match;
		std::regex pattern(R"((\d+)\s+(\d+))");
		if (std::regex_search(line, match, pattern)) {
			if (match.size() < 3) return;
			left.push_back(std::atoi(match[1].str().c_str()));
			right.push_back(std::atoi(match[2].str().c_str()));
		}
	}

	if (left.size() != right.size()) return;
	std::sort(left.begin(), left.end());
	std::sort(right.begin(), right.end());
}

void write_sln(const std::string& name, const std::string& sln) {
	std::ofstream ofs(name, std::ios::trunc);
	ofs << sln;
}

void part_1() {
	std::vector<int> left, right;
	get_vectors(R"(input_day1.txt)", left, right);
	int total_sum = 0;
	for (int i = 0; i < left.size(); i++) {
		total_sum += std::abs(left[i] - right[i]);
	}
	write_sln("day1_sln_p1.txt", std::to_string(total_sum));
}

void part_2() {
	std::vector<int> left, right;
	get_vectors(R"(D:\AOC\2024\01\input.txt)", left, right);
	auto num_of_occurances = [](int number, const std::vector<int>& right) {
		int occurances = 0;
		for (const auto& num : right) {
			if (num == number) occurances++;
		}
		return occurances;
	};
	int total_sum = 0;
	for (const auto& num : left) {
		int occ = num_of_occurances(num, right);
		total_sum += occ * num;
	}
	write_sln("day1_sln_p2.txt", std::to_string(total_sum));
}

int main() {
	part_1();
	part_2();
	return 0;
}