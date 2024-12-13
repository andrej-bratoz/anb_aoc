#include <fstream>
#include <iostream>
#include <regex>
#include <vector>
#include <sstream>
#include <map>
#include <locale>
#include <set>

struct garden_item {
	int pos_x;
	int pos_y;
	int plot_count;
	char id;
	int aid;

	garden_item(int x, int y, char _id) : pos_x(x), pos_y(y), id(_id), plot_count(4), aid(-1) {}
	garden_item(int x, int y, char _id, int pc) : pos_x(x), pos_y(y), id(_id), plot_count(pc), aid(-1) {}
	garden_item() : pos_x(0), pos_y(0), id(0), plot_count(0), aid(-1)  {}
	garden_item(const garden_item& gi) : pos_x(gi.pos_x), pos_y(gi.pos_y), id(gi.id), 
										 plot_count(gi.plot_count), aid(gi.area_id()) {}

	int area_id() const { return aid; }
	void area_id(int id) { aid = id; }
	void inc_plot_count() { plot_count++; }
	void dec_plot_count() { plot_count--; }
	void set_plot_count(int count) { plot_count = count; }
	bool valid() const { return id != 0; }
	void mark() { _marked = true; }
	void unmark() { _marked = false; }
	bool marked() const { return _marked; }

	bool operator==(const garden_item& gi) const { return id == gi.id; }
	private: 
	bool _marked = false;

};

struct magic_garden {

	int area_id = 99;
	std::vector<std::vector<garden_item>> garden;
	magic_garden(const std::vector<std::string> input_data) {
		for (int y = 0; y < input_data.size(); y++) {
			for (int x = 0; x < input_data[y].length(); x++) {
				if (garden.size() <= y) { garden.push_back({ std::vector<garden_item>() });}
				garden[y].push_back(garden_item(x, y, input_data[y][x]));
			}
		}
	}

	int next_area_id() { area_id++; return area_id;  }

	#define at(x,y) garden[y][x]

	void calculate_fence_required_base() {
		for (int y = 0; y < garden.size(); y++) {
			for (int x = 0; x < garden[y].size(); x++) {	 // calculate left to right
				auto item = garden[y][x];

				auto above_gi = above(item);
				if (above_gi.valid()) {
					if (above_gi == item) {
						item.dec_plot_count();
					}
				}
				//
				auto below_gi = below(item);
				if (below_gi.valid()) {
					if (below_gi == item) {
						item.dec_plot_count();
					} // if they are different it will be already handled in above case

				}
				//
				auto left_gi = left(item);
				if (left_gi.valid()) {
					if (left_gi == item) {
						item.dec_plot_count();
					}
				}
				//
				auto right_gi = right(item);
				if (right_gi.valid()) {
					if (right_gi == item) {
						item.dec_plot_count();
					}
				}
				garden[y][x] = item;
			}
		}
	}

	garden_item find_unmarked() {
		for (int y = 0; y < garden.size(); y++) {
			for (int x = 0; x < garden[y].size(); x++) {
				if (!garden[y][x].marked()) {
					return garden[y][x];
				}
			}
		}
		return {};
	}

	int walk_to_same_neighbours(int area_id, garden_item& gi) {
		int total = 1;
		int pos_x = gi.pos_x;
		int pos_y = gi.pos_y;
		gi.area_id(area_id);
		gi.mark();
		garden[pos_y][pos_x] = gi;
		if (pos_x > 0) {
			auto& item_to_left = at(pos_x - 1, pos_y);
			if (item_to_left == gi && item_to_left.area_id() != area_id) {
				total += walk_to_same_neighbours(area_id, item_to_left);
			}
		}
		if (pos_x < garden[pos_y].size() - 1) {
			auto& item_to_right = at(pos_x + 1, pos_y);
			if (item_to_right == gi && item_to_right.area_id() != area_id) {
				total += walk_to_same_neighbours(area_id, item_to_right);
			}
		}
		if (pos_y > 0) {
			auto& item_at_top = at(pos_x, pos_y - 1);
			if (item_at_top == gi && item_at_top.area_id() != area_id) {
				total += walk_to_same_neighbours(area_id, item_at_top);
			}
		}
		if (pos_y < garden.size() - 1) {
			auto& item_below = at(pos_x, pos_y + 1);
			if (item_below == gi && item_below.area_id() != area_id) {
				total += walk_to_same_neighbours(area_id, item_below);
			}
		}
		return total;
	}

	int total_fence_required() {
		int total = 0;
		int root_area_id = area_id + 1;
		calculate_fence_required_base();
		while (true) {
			auto item = find_unmarked();
			if (!item.valid()) break;
			int next_aid = next_area_id();
			walk_to_same_neighbours(next_aid, item);
		}
		int max_area_id = area_id;
		for (int i = root_area_id; i <= max_area_id; i++) {
			auto area = all_garden_with_area_id(i);
			int fence_raw = 0;
			for (const auto& itm : area) {
				fence_raw += itm.plot_count;
			}
			int multiplied = fence_raw * area.size();
			total += multiplied;
		}
		return total;
	}

	int total_fence_required_with_bulk_discount() {
		int total = 0;
		int root_area_id = area_id + 1;
		calculate_fence_required_base();
		while (true) {
			auto item = find_unmarked();
			if (!item.valid()) break;
			int next_aid = next_area_id();
			walk_to_same_neighbours(next_aid, item);
		}
		int max_area_id = area_id;
		for (int i = root_area_id; i <= max_area_id; i++) {
			auto area = all_garden_with_area_id(i);
			//for (int idx = 0; idx < area.size(); idx++) { area[idx].unmark(); }
			int fence_raw = 0;
			// count all the edges
			for (const auto& idx : area) {

				if (above(idx) != idx && left(idx) != idx) {
					fence_raw++;
				}

				if (above(idx) != idx && right(idx) != idx) {
					fence_raw++;
				}

				if (below(idx) != idx && right(idx) != idx) {
					fence_raw++;
				}

				if (below(idx) != idx && left(idx) != idx) {
					fence_raw++;
				}

				/*
					E E
					E X
				*/
				if (right(idx) == idx && below(idx) == idx && below(right(idx)) != idx) {
					fence_raw++;
				}

				/*
					E X
					E E
				*/
				if (above(idx) == idx && right(idx) == idx && right(above(idx)) != idx) {
					fence_raw++;
				}

				/*
					E E
					X E
				*/
				if (below(idx) == idx && left(idx) == idx && left(below(idx)) != idx) {
					fence_raw++;
				}

				/*
					X E
					E E
				*/
				if (above(idx) == idx && left(idx) == idx && left(above(idx)) != idx) {
					fence_raw++;
				}
			}			
			int multiplied = fence_raw * area.size();
			total += multiplied;
		}
		return total;
	}

	std::vector<garden_item> all_garden_with_area_id(int area_id) {
		std::vector<garden_item> result;
		for (int y = 0; y < garden.size(); y++) {
			for (int x = 0; x < garden[y].size(); x++) {
				if (garden[y][x].area_id() == area_id) result.emplace_back(garden[y][x]);
			}
		}
		return result;
	}

	garden_item above(const garden_item& in) {
		if (!in.valid()) return {};
		if (in.pos_y == 0)  return {};
		return garden[in.pos_y - 1][in.pos_x];
	}

	garden_item below(const garden_item& in) {
		if (!in.valid()) return {};
		if (in.pos_y == garden.size() - 1) return {};
		return garden[in.pos_y + 1][in.pos_x];
	}

	garden_item right(const garden_item& in) {
		if (!in.valid()) return {};
		if (in.pos_x == garden[in.pos_y].size() - 1)  return {}; 
		return garden[in.pos_y][in.pos_x + 1];
	}

	garden_item left(const garden_item& in) {
		if (!in.valid()) return {};
		if (in.pos_x == 0) return {};
		return garden[in.pos_y][in.pos_x -1];
	}
};

void write_sln(const std::string& name, const std::string& sln) {
	std::ofstream ofs(name, std::ios::trunc);
	ofs << sln;
}

void load_data(const std::string& file, std::vector<std::string>& lines) {
    std::ifstream ifs(file);
	if (!ifs.is_open()) { return; }
	std::string line;
	while(std::getline(ifs, line)) {
        lines.push_back(line);
    }
}
void part_1() { 
    std::vector<std::string> lines;
    load_data("input_day12.txt", lines);
	magic_garden mg(lines);
	int total = mg.total_fence_required();
    write_sln("solution_day12_p1.txt", std::to_string(total)); 
}
void part_2() { 
    std::vector<std::string> lines;
    load_data("input_day12.txt", lines);
	magic_garden mg(lines);
	int total = mg.total_fence_required_with_bulk_discount();
    write_sln("solution_day12_p2.txt", std::to_string(total));
}
int main() {
    part_1();
    part_2();
    return 0;
}
