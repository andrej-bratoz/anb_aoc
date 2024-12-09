#include <fstream>
#include <iostream>
#include <regex>
#include <vector>
#include <limits>
#include <sstream>

#include <algorithm> 
#include <cctype>
#include <locale>


struct disk_item {
	bool is_free = true;
	int size = 0;
	int id = -1;
	int index = -1;

	disk_item(int s, bool f) : size(s), is_free(f) {}
	disk_item(int s, int i, bool f) : size(s), id(i), is_free(f) {}
	bool operator==(const disk_item& d) {
		return d.is_free == is_free && size == d.size && id == d.id;
	}

};

struct hdd {
	std::vector<disk_item> items;
	
	hdd(const std::vector<disk_item>& vec) : items(vec) {}
	void create_raw() {
		raw.clear();
		int index = 0;
		int write_idx = 0;
		for (const auto& item : items) {
			if (item.is_free) push_back(".", item.size);
			else {
				push_back(std::to_string(item.id), item.size);
				write_idx++;
			}
			index++;
		}
	}
	int free_space_at_index(int index) {
		int free_space = 0;
		for (int i = index; i < raw.size(); i++) {
			if (raw[i] == ".") free_space++;
			else break;
		}
		return free_space;
	}

	void order() {
		while (true) {
			int fe = find_first_empty();
			int le = find_last_full();
			if (fe > le) break;
			auto val = raw[fe];
			raw[fe] = raw[le];
			raw[le] = val;
		}
	}

	disk_item& first_space_of_size(int size) {
		auto val = std::find_if(items.begin(), items.end(), [&size](const disk_item& i2) { return i2.is_free && i2.size >= size; });
		if (val != items.end()) {
			return *val;
		}
		throw 1;
	}
	
	void order_whole_files() {
		for (int i = items.size() - 1; i >= 0; i--) {
			if (items[i].is_free) continue;
			disk_item di = items[i];
			try {
				disk_item space = first_space_of_size(di.size);
				if (space.index >= di.index) continue;
				if (space.size == di.size) { 

					items[space.index].id = di.id;
					items[space.index].is_free = false;
					items[di.index].id = -1;
					items[di.index].is_free = true;
				} 
				else if(space.size > di.size) {
					disk_item new_space(space.size - di.size, -1, true);
					new_space.index = space.index + 1;
					items[space.index] = di;
					items[di.index].is_free = true;
					items[di.index].id = -1;
					items.insert(items.begin() + space.index + 1, new_space);					
				}
			}
			catch (...) {  }
			for (int i = 0; i < items.size(); i++) {
				items[i].index = i;
			}
		}
	}

	unsigned long long total_val() {
		unsigned long long total = 0;
		int index = 0;
		for (const auto& item : raw) {
			if (item == ".") break;
			total += atoll(item.c_str()) * index;
			index++;
		}
		return total;
	}

	unsigned long long total_val2() {
		unsigned long long total = 0;
		int index = 0;
		for (const auto& item : raw) {
			if (item != ".") {
				total += atoll(item.c_str()) * index;
			}
			index++;
		}
		return total;
	}

	int find_first_empty() {
		int ind = 0;
		for (const auto& x : raw) {
			if (x == ".") return ind;
			ind++;
		}
	}

	int find_last_full() {
		for (int i = raw.size() - 1; i >= 0; i--) {
			if (raw[i] != ".") return i;
		}
		return -1;
	}

	std::string raw_as_string() {
		std::string result;
		for (const auto& s : raw) {
			result += s;
		}
		return result;
	}

	void push_back(const std::string& s, int count) {
		for (int i = 0; i < count; i++) {
			raw.push_back(s);
		}
	}
	std::vector<std::string> raw;
};


void load_data(const std::string& file, std::vector<disk_item>& items) {
	std::ifstream ifs(file);
	if (!ifs.is_open()) { return; }
	std::string line;
	int row = 0;
	std::getline(ifs, line);
	bool is_data = false;
	int index = 0;
	int abs_idx = 0;
	for (auto& ch : line) {
		disk_item di(ch - '0', !is_data ? index : -1, is_data);
		if(is_data) index++;
		is_data = !is_data;
		di.index = abs_idx;
		items.push_back(di);
		abs_idx++;
	}
}

void write_sln(const std::string& name, const std::string& sln) {
	std::ofstream ofs(name, std::ios::trunc);
	ofs << sln;
}

void part_1() {
	unsigned long long target = 0;
	std::vector<disk_item> items;
	load_data("input_day9.txt", items);
	hdd disk(items);
	disk.create_raw();
	disk.order();
	auto x = disk.raw_as_string();
	target = disk.total_val();
	write_sln("solution_day9_p1.txt", std::to_string(target));
}

void part_2() {
	unsigned long long target = 0;
	std::vector<disk_item> items;
	load_data("input_day9.txt", items);
	hdd disk(items);
	disk.order_whole_files();
	disk.create_raw();
	target = disk.total_val2();
	write_sln("solution_day9_p2.txt", std::to_string(target));
}

int main() {
	part_1();
	part_2();
	return 0;
}
