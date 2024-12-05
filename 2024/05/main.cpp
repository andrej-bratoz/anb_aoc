#include <fstream>
#include <iostream>
#include <sstream>
#include <regex>
#include <vector>
#include <math.h>

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

struct node {
	node(int name) : _id(name) {}
	bool operator==(int node) const { return _id == node; }
	bool operator==(const node& n) const { return _id == n._id; }
	void insert_after(int id) {
		if(std::find(_after.cbegin(), _after.cend(),id) == _after.cend()) {
			_after.emplace_back(id);
		}
	}
	void insert_before(int id) {
		if (std::find(_before.cbegin(), _before.cend(), id) == _before.cend()) {
			_before.emplace_back(id);
		}
	}
	int id() const { return _id; }

	private:
	int _id;
	std::vector<node> _before;
	std::vector<node> _after;

};

struct book {
	void add_node_before_node(int before, int after) {
		create_node_if_required(before);
		create_node_if_required(after);
		find(before).insert_after(after);
		find(after).insert_before(before);
	}

	void copy_rules(const book& book) {
		_order_rules = book._order_rules;
	}

	void set_pages(const std::vector<int>& pages) {
		_pages = pages;
	}

	bool is_valid() const {
		
	}

	private:
	void create_node_if_required(int id) {
		std::vector<node>::iterator result = std::find(_order_rules.begin(), _order_rules.end(), id);
		if(result == _order_rules.end()) {
			_order_rules.emplace_back(id);
		}
	}
	node& find(int id) {
		const auto result = std::find(_order_rules.begin(), _order_rules.end(), id);
		return *result;
	} 
	std::vector<node> _order_rules;
	std::vector<int> _pages;
};

std::vector<int> parse_pages(const std::string& line) {
	std::vector<int> result;
	auto pages = split(line, ",");
	for(const auto& page : pages) {
		if (page.length()) {
			result.push_back(atoi(page.c_str()));
		}
	}
	return result;

}

void load_data(const std::string& file, std::vector<book>& books) {
	std::ifstream ifs(file);
	if (!ifs.is_open()) { return; }
	std::string line;
	bool is_seq = false;
	book bok;
	while (std::getline(ifs, line)) {
		if(!is_seq) {
			auto rule = split(line, "|");
			if (line.length()) {
				int before = atoi(rule[0].c_str());
				int after = atoi(rule[1].c_str());
				bok.add_node_before_node(before, after);
				continue;
			}
		}
		if (!line.length()) {
			is_seq = !is_seq;
			continue;
		}
		if (is_seq) {
		
			if (books.empty()) {
				bok.set_pages(parse_pages(line));
				books.push_back(bok);
			}
			else {
				book b;
				b.copy_rules(bok);
				b.set_pages(parse_pages(line));
				books.push_back(b);
			}
		}
	}
}


void write_sln(const std::string& name, const std::string& sln) {
	std::ofstream ofs(name, std::ios::trunc);
	ofs << sln;
}

void part_1() {
	std::vector<book> books;
	load_data("input_day5.txt", books);
}

void part_2() {
	
}

int main() {
	part_1();
	part_2();
	return 0;
}