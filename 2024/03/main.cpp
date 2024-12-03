#include <fstream>
#include <iostream>
#include <string>
#include <regex>
#include <vector>

struct mul {
	mul(int op1, int op2) : op_1(op1), op_2(op2){}
	mul(int op1, int op2, bool enable) : op_1(op1), op_2(op2), m_enabled(enable) {}
	int operator()() const { return m_enabled ? (op_1 * op_2) : 0; }
	private:
	int op_1, op_2;
	bool m_enabled = true;
};

void get_vectors2(const std::string& file, std::vector<mul>& reports) {
	std::ifstream ifs(file);
	if (!ifs.is_open()) { return; }

	std::string line;
	bool enabled = true;
	while (std::getline(ifs, line))
	{
		std::regex r(R"(mul\(\d+,\d+\)|do\(\)|don't\(\))");
		for (std::smatch sm; std::regex_search(line, sm, r);) {
			auto mtch = sm.str();
			if (mtch == "do()") {
				enabled = true;
				line = sm.suffix();
				continue;
			}
			if(mtch == "don't()") {
				enabled = false;
				line = sm.suffix();
				continue;
			}
			std::smatch nums;
			std::regex r2("(\\d+),(\\d+)");
			std::regex_search(mtch, nums, r2);
			reports.emplace_back(std::atoi(nums[1].str().c_str()), std::atoi(nums[2].str().c_str()),enabled);
			line = sm.suffix();
		}
	}
}

void get_vectors1(const std::string& file, std::vector<mul>& reports) {
	std::ifstream ifs(file);
	if (!ifs.is_open()) { return; }

	std::string line;
	while (std::getline(ifs, line)) {
		std::regex r(R"(mul\(\d+,\d+\))");
		for (std::smatch sm; std::regex_search(line, sm, r);) {
			auto numbers = sm.str();
			std::smatch nums;
			std::regex r2("(\\d+),(\\d+)");
			std::regex_search(numbers, nums, r2);
			reports.emplace_back(std::atoi(nums[1].str().c_str()), std::atoi(nums[2].str().c_str()));
			line = sm.suffix();
		}
	}
}

void write_sln(const std::string& name, const std::string& sln) {
	std::ofstream ofs(name, std::ios::trunc);
	ofs << sln;
}

void part_1() {
	std::vector<mul> muls;
	get_vectors1(R"(input_day3.txt)", muls);
	int total_sum = 0;
	for (auto& value : muls) {
		total_sum += value();
	}
	write_sln("day3_sln_p1.txt", std::to_string(total_sum));
}

void part_2() {
	std::vector<mul> muls;
	get_vectors2(R"(input_day3.txt)", muls);
	int total_sum = 0;
	for (auto& value : muls) {
		total_sum += value();
	}
	write_sln("day3_sln_p2.txt", std::to_string(total_sum));
}

int main() {
	part_1();
	part_2();
	return 0;
}