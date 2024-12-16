#include <fstream>
#include <iostream>
#include <regex>
#include <vector>
#include <sstream>
#include <stack>
#include <locale>
#include <map>
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

enum class dir {
	west, north, east, south
};

struct reindeer {
	int x, y;
	bool bricked;
	dir current_orientation;
	reindeer(const int pos_x, const int pos_y, dir co, bool b) : x(pos_x), y(pos_y), bricked(b), current_orientation(co) {}
	reindeer() : x(-1), y(-1), bricked(false), current_orientation(dir::east) {}
};

struct item {
	enum class type {
		wall, space
	};
	type item_type;
	int x, y;
	item(int pos_x, int pos_y, type t) : item_type(t), x(pos_x), y(pos_y) {}

};

struct labyrinth {
	std::vector<std::vector<item>> items;
	int exit_x, exit_y;
	dir current_orientation = dir::east;
	reindeer rd;
	std::stack<dir> moves;

		labyrinth(const std::vector<std::string>& input) {
		int y = 0;
		for(const auto& line : input) {
			int x = 0;
			std::vector<item> ln_;
			for(const auto itm : line) {
				if(itm == '#') {
					ln_.emplace_back(x,y,item::type::wall);
				} else {
					ln_.emplace_back(x,y,item::type::space);
					if(itm == 'S') {
						rd.x = x; rd.y = y;
					} else if(itm == 'E') {
						exit_x = x;
						exit_y = y;
					}
				}
				x++;
			}
			items.push_back(ln_);
			y++;
		}
	}

	void backtrack() {
		if(!moves.empty())
			moves.pop();
	}

	dir last_move() {
		return moves.top();
	}

	dir opposite_move(dir d) {
		switch (d) {
		case dir::west:
			return dir::east;
		case dir::north:
			return dir::south;
		case dir::east:
			return dir::west;
		case dir::south:
			return dir::north;
		}
		throw - 1;
	}

	reindeer move_reindeer(reindeer crd, const dir d, std::stack<dir>& move_stack) {
		switch (d) {
		case dir::west:
			crd.x--;
			crd.current_orientation = dir::west;
			move_stack.push(dir::west);
			std::cout << "Moving W\n";
			break;
		case dir::north:
			crd.y--;
			move_stack.push(dir::north);
			crd.current_orientation = dir::north;
			std::cout << "Moving N\n";
			break;
		case dir::east:
			crd.x++;
			move_stack.push(dir::east);
			crd.current_orientation = dir::east;
			std::cout << "Moving E\n";
			break;
		case dir::south:
			crd.y++;
			crd.current_orientation = dir::south;
			move_stack.push(dir::south);
			std::cout << "Moving S\n";
			break;
		}
		return crd;
	}

	std::vector<dir> acceptable_directions(dir d) {
		switch (d) {
		case dir::west:
			return { dir::west, dir::south, dir::north };
		case dir::north:
			return { dir::north, dir::west, dir::east};
		case dir::east:
			return { dir::east, dir::north, dir::south};
		case dir::south:
			return { dir::south, dir::west, dir::west};
		}
	}

	bool is_dead_end(reindeer rd) {
		if (items[rd.y][rd.x].item_type == item::type::wall) return true;

		switch(rd.current_orientation) {
			case dir::west: {
				if(items[rd.y][rd.x -1].item_type == item::type::wall && // before me
					items[rd.y-1][rd.x].item_type == item::type::wall && // above me
					items[rd.y + 1][rd.x].item_type == item::type::wall) /* below me */ return true;
			}
			case dir::north:
				if (items[rd.y - 1][rd.x].item_type == item::type::wall && // before me
					items[rd.y][rd.x + 1].item_type == item::type::wall && // to the right
					items[rd.y][rd.x - 1].item_type == item::type::wall) /*to the left */ return true;
			case dir::east:
				if (items[rd.y][rd.x + 1].item_type == item::type::wall && // before me
					items[rd.y - 1][rd.x].item_type == item::type::wall && // above me
					items[rd.y + 1][rd.x].item_type == item::type::wall) /* below me */ return true;
			case dir::south:
				if (items[rd.y + 1][rd.x].item_type == item::type::wall && // before me
					items[rd.y][rd.x - 1].item_type == item::type::wall && // to the right
					items[rd.y][rd.x + 1].item_type == item::type::wall) /*to the left */ return true;
		}
		return false;
	}

	void move(reindeer rd /*make copy for each move*/, int current_score, int& best_score, std::stack<dir> vec) {

		if(rd.x == exit_x && rd.y == exit_y) {
			if(current_score < best_score) {
				best_score = current_score;
			}
			return;
		}
		if (is_dead_end(rd) || rd.bricked) {
			//delete moves;
			return;
		}

		// initial facing east
		auto dirs = acceptable_directions(rd.current_orientation);
		for (auto dir : dirs) {
			auto xx = move_reindeer(rd, dir, vec);
			if (xx.x == rd.x && xx.y == rd.y) {
				backtrack();
				continue;
			}
			move(xx, current_score + 1, best_score, vec);
			backtrack();
		}
	}

	void move() {
		int best_score = 0;
		move(rd, 0, best_score, {});
		int i = 0;
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
    load_data("input_day16.txt", lines);
	labyrinth lab(lines);
	lab.move();
    write_sln("solution_day16_p1.txt", ""); 
}
void part_2() { 
    std::vector<std::string> lines;
    load_data("input_day16.txt", lines);
    write_sln("solution_day16_p2.txt", ""); 
}
int main() {
    part_1();
    part_2();
    return 0;
}
