#include <fstream>
#include <iostream>
#include <regex>
#include <vector>
#include <limits>
#include <sstream>

#include <algorithm> 
#include <cctype>
#include <locale>

// thank you SO [https://stackoverflow.com/a/217605/732348]
// trim from start (in place)
inline void ltrim(std::string& s) {
	s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](unsigned char ch) {
		return !std::isspace(ch);
		}));
}

// trim from end (in place)
inline void rtrim(std::string& s) {
	s.erase(std::find_if(s.rbegin(), s.rend(), [](unsigned char ch) {
		return !std::isspace(ch);
		}).base(), s.end());
}

inline void trim(std::string& s) {
	rtrim(s);
	ltrim(s);
}

// It's weekend, let's take a crutch [https://www.geeksforgeeks.org/find-all-combinations-that-adds-upto-given-number-2/]

/*  arr - array to store the combination
	index - next location in array
	num - given number
	reducedNum - reduced number */
void findCombinationsUtil(int arr[], int index,
	int num, int reducedNum, std::vector<std::vector<int>>& results)
{
	// Base condition
	if (reducedNum < 0)
		return;

	// If combination is found, print it
	if (reducedNum == 0)
	{
		std::vector<int> res;
		for (int i = 0; i < index; i++) {
			//std::cout << arr[i] << " ";
			res.push_back(arr[i]);
		}
		//std::cout << std::endl;
		results.push_back(res);
		return;
	}

	// Find the previous number stored in arr[]
	// It helps in maintaining increasing order
	int prev = (index == 0) ? 1 : arr[index - 1];

	// note loop starts from previous number
	// i.e. at array location index - 1
	for (int k = prev; k <= num; k++)
	{
		// next element of array is k
		arr[index] = k;

		// call recursively with reduced number
		findCombinationsUtil(arr, index + 1, num,
			reducedNum - k, results);
	}
}

/* Function to find out all combinations of
   positive numbers that add upto given number.
   It uses findCombinationsUtil() */
void findCombinations(int n, std::vector<std::vector<int>>& result)
{
	// array to store the combinations
	// It can contain max n elements
	std::vector<int> arr(n);
	//find all combinations
	return findCombinationsUtil(arr.data(), 0, n, n, result);
}

// thank you, SO [https://stackoverflow.com/a/46931770/732348]
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
}


struct arith_line {
	unsigned long long _target;
	std::vector<unsigned long long> _operands;
	std::vector<unsigned long long> _operands_r;

	arith_line(unsigned long long target, std::vector<unsigned long long> operands) : _target(target), _operands(operands), _operands_r(operands){
		std::reverse(_operands_r.begin(), _operands_r.end());
	}

	void print(const std::vector<char>& ops) const {
		std::stringstream ss;
		ss << _target << " = ";
		int idx = -1;
		for (auto op : _operands) {
			if (idx >= 0) {
				ss << ops[idx] << " ";
			}
			ss << op << " ";
			idx++;
		}
		std::cout << ss.str() << "\n";
	}

	void print_op(const std::vector<char>& ops) const {
		for (auto op : ops) {
			std::cout << op << " ";
		}
		std::cout << "\n";
	}

	void print_op(const std::vector<int>& ops) const {
		for (auto op : ops) {
			std::cout << op << " ";
		}
		std::cout << "\n";
	}

	void print(const char ch) const {
		std::stringstream ss;
		ss << _target << " = ";
		int idx = 0;
		for (auto op : _operands) {
			ss << op << " ";
			if (idx < _operands.size() - 1) {
				ss << ch << " ";
			}
			idx++;
		}
		std::cout << ss.str() << "\n\n";
	}

	bool possible3() const {
		int muls = _operands.size() - 1;
		int pluses = 0;
		int concat = 0;
		int next_sub = 0;
		int orig_muls = muls;
		bool flip = true;

		std::vector<std::vector<int>> results;
		findCombinations(muls, results);

		for(auto& vec : results){
			if (vec.size() <= 3) {
				while (vec.size() < 3) {
					vec.push_back(0);
				}
			}
			else { 
				continue;
			}
			std::vector<int> amounts{ vec[0], vec[1], vec[2]};
			std::vector<char> _ops;
			std::sort(amounts.begin(), amounts.end());
			do {
				_ops.clear();
				for (int i = 0; i < amounts[0]; i++) { _ops.push_back('*'); }
				for (int i = 0; i < amounts[1]; i++) { _ops.push_back('+'); }
				for (int i = 0; i < amounts[2]; i++) { _ops.push_back('|'); }
				std::sort(_ops.begin(), _ops.end());
				do {
					do {
						if (calculate2(_ops) == _target) {
							return true;
						}
					} while (std::next_permutation(_ops.begin(), _ops.end()));
				} while (std::prev_permutation(_ops.begin(), _ops.end()));
			} while (std::next_permutation(amounts.begin(), amounts.end()));
		}
		return false;
	}

	bool possible() const {
		// check edge cases
		unsigned long long all_mul = 1;
		unsigned long long all_add = 0;

		for (auto num : _operands) {
			all_mul *= num;
			all_add += num;
		}
		// all operands of the same type '*'
		if (all_mul == _target) {
			return true;
		}

		// all operands of the same type '+'
		if (_target == all_add) {
			return true;
		}

		int muls = 1;
		while (muls < _operands.size() - 1) {
			std::vector<char> _ops;
			for (int i = 0; i < _operands.size() - 1 - muls; i++) { _ops.push_back('*'); }
			for (int i = 0; i < muls; i++) { _ops.push_back('+'); }
			do {
				if (calculate(_ops, _target) == _target) {
					return true;
				}
			} while (std::next_permutation(_ops.begin(), _ops.end()));
			muls++;
			//			
		}
		return false;
	}

	private:
	unsigned long long calculate(const std::vector<char>& ops, unsigned long long target) const {
		unsigned long long result = _operands[0];
		int idx = 1;
		for (const auto& op : ops) {
			if (result > target) return -1;
			if (op == '+') {
				result += _operands[idx];
			}
			else {
				result *= _operands[idx];
			}
			idx++;
		}
		return result;
	}

	unsigned long long calculate2(const std::vector<char>& ops) const {
		
		std::string res = "";
		unsigned long long result = _operands[0];
		int idx = 1;
		for (const auto& op : ops) {
			if (op == '+') {
				result += _operands[idx];
			}
			else if (op == '|') {
				{
					std::stringstream ss;
					ss << result << _operands[idx];
					res = ss.str();
				}
				result = atoll(res.c_str());
			}
			else {
				result *= _operands[idx];
			}
			idx++;
		}
		return result;
	}
};


void load_data(const std::string& file, std::vector<arith_line>& lns) {
	std::ifstream ifs(file);
	if (!ifs.is_open()) { return; }
	std::string arith_line;
	int row = 0;
	while (std::getline(ifs, arith_line)) {
		auto parts = split(arith_line, ":");
		unsigned long long target = atoll(parts[0].c_str());
		std::vector<unsigned long long> ops;
		auto operands = split(parts[1], " ");
		for (auto& op : operands) {
			if (!op.length()) continue;
			trim(op);
			ops.push_back(atoll(op.c_str()));
		}
		lns.push_back({ target, ops });
	}
}

void write_sln(const std::string& name, const std::string& sln) {
	std::ofstream ofs(name, std::ios::trunc);
	ofs << sln;
}

void part_1() {
	unsigned long long target = 0;
	std::vector<arith_line> lines;
	load_data("input_day7.txt", lines);
	for (const auto& line : lines) {
		if (line.possible()) {
			target += line._target;
		}
	}
	write_sln("solution_day7_p1.txt", std::to_string(target));
}

void part_2() {
	unsigned long long target = 0;
	std::vector<arith_line> lines;
	load_data("input_day7.txt", lines);
	for (const auto& line : lines) {
		if (line.possible() || line.possible3()) {
			target += line._target;
		}
	}
	write_sln("solution_day7_p2.txt", std::to_string(target));
}

int main() {
	part_1();
	part_2();
	return 0;
}
