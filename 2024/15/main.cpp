#include <fstream>
#include <iostream>
#include <regex>
#include <vector>
#include <sstream>
#include <map>
#include <locale>


struct item {
	enum class type {
		wall, box, empty_space
	};
	type item_type;
	int x = -1;
	int y = -1;
	char raw;
	item(const item&) = default;
	item(item::type type, int _x, int _y, char r) : item_type(type), x(_x), y(_y), raw(r) {}


};

struct move {
	char mv;
	move(char c) : mv(c) {}
	move(const move&) = default;
};

struct robot {
	int pos_x;
	int pos_y;
	robot(const robot&) = default;
	robot() : pos_x(-1), pos_y(-1) {}
	robot(const int x, const int y) : pos_x(x), pos_y(y) {}
};


struct warehouse {
	std::vector<std::vector<item>> items;
	std::vector<move> moves;
	bool is_map = true;
	robot wh_robot;
	warehouse(const std::vector<std::string>& lines, bool twice) {
		if (twice) {
			int y = 0, x = 0;;
			for (const auto& lne : lines) {
				if (lne.empty()) {
					is_map = false;
					continue;
				}
				std::vector<item> itm_line; x = 0;
				for (const auto& itm : lne) {
					if (is_map) {
						if (itm == '.') {
							itm_line.push_back({ item::type::empty_space, x,y, itm });
							itm_line.push_back({ item::type::empty_space, x + 1,y, itm });
						}
						else if (itm == 'O') {
							itm_line.push_back({ item::type::box, x,y, '[' });
							itm_line.push_back({ item::type::box, x + 1,y, ']' });
						}
						else if (itm == '@') {
							itm_line.push_back({ item::type::empty_space, x, y, '.' });
							itm_line.push_back({ item::type::empty_space, x + 1, y, '.' });
							wh_robot = { x,y };
						}
						else {
							itm_line.push_back({ item::type::wall, x, y, itm });
							itm_line.push_back({ item::type::wall, x + 1, y, itm });
						}
					}
					else {
						moves.push_back({ itm });
					}
					x += 2;;
				}
				if (!itm_line.empty()) items.push_back(itm_line);
				y++;
			}
		}
		else {
			int y = 0, x = 0;;
			for (const auto& lne : lines) {
				if (lne.empty()) {
					is_map = false;
					continue;
				}
				std::vector<item> itm_line; x = 0;
				for (const auto& itm : lne) {
					if (is_map) {
						if (itm == '.') {
							itm_line.push_back({ item::type::empty_space, x,y, itm });
						}
						else if (itm == 'O') {
							itm_line.push_back({ item::type::box, x,y, itm });
						}
						else if (itm == '@') {
							itm_line.push_back({ item::type::empty_space, x, y, '.' });
							wh_robot = { x,y };
						}
						else {
							itm_line.push_back({ item::type::wall, x, y, itm });
						}
					}
					else {
						moves.push_back({ itm });
					}
					x++;
				}
				if (!itm_line.empty()) items.push_back(itm_line);
				y++;
			}
		}
	}

	item& at(int x, int y) {
		return items[y][x];
	}

	void move(int x1, int y1, int x, int y) {
		auto itm1 = items[y1][x1];
		auto itm2 = items[y][x];

		itm1.x = x1;
		itm1.y = y1;
		//
		itm2.x = x;
		itm2.y = y;
		
		items[y1][x1] = itm2;
		items[y][x] = itm1;
	}

	void print() {
		int y = 0;
		for (const auto& item_line : items) {
			int x = 0;
			for (const auto& itm : item_line) {
				if (wh_robot.pos_x == x && wh_robot.pos_y == y) {
					std::cout << "@";
				}
				else {
					std::cout << itm.raw;
				}
				x++;
			}
			y++;
			std::cout << "\n";
		}

		std::cout << "===========\n";
	}

	int move() {
		//print();
		for (const auto& move : moves) {
			switch (move.mv) {
				case '>': { try_move_right(); break; }
				case '<': { try_move_left(); break; }
				case '^': { try_move_up(); break; }
				case 'v': { try_move_down(); break; }
				default: throw - 1;
			}
			//print();
		}
		int total = 0;
		for (int y = 0; y < items.size(); y++) {
			for (int x = 0; x < items[y].size(); x++) {
				if (items[y][x].item_type == item::type::box) {
					total += (100 * y + x);
				}
			}
		}
		return total;
	}

	int move2() {
		print();
		for (const auto& move : moves) {
			switch (move.mv) {
			case '>': { try_move_right(); break; }
			case '<': { try_move_left(); break; }
			case '^': { try_move_up(); break; }
			case 'v': { try_move_down(); break; }
			default: throw - 1;
			}
			print();
		}
		int total = 0;
		for (int y = 0; y < items.size(); y++) {
			for (int x = 0; x < items[y].size(); x++) {
				if (items[y][x].item_type == item::type::box) {
					total += (100 * y + x);
				}
			}
		}
		return total;
	}

	void try_move_right() {
		int wall_pos_x = -1;
		int free_space_pos_x;
		int free_spaces = 0;

		if (items[wh_robot.pos_y][wh_robot.pos_x + 1].item_type == item::type::empty_space) {
			wh_robot.pos_x++;
			return;
		}

		for (int i = wh_robot.pos_x + 1; i < items[wh_robot.pos_y].size(); i++) {
			if (items[wh_robot.pos_y][i].item_type == item::type::wall) {
				wall_pos_x = i;
				break;
			}
			else if (items[wh_robot.pos_y][i].item_type == item::type::empty_space) {
				free_space_pos_x = i;
				free_spaces++;
				break;
			}
		}
		if (free_spaces == 0) return;
		for (int i = free_space_pos_x; i > wh_robot.pos_x; i--) {
			this->move(i, wh_robot.pos_y, /**/ i - 1, wh_robot.pos_y);
		}
		wh_robot.pos_x++;
	}

	void try_move_left() {
		int wall_pos_x = -1;
		int free_space_pos_x;
		int free_spaces = 0;

		if (items[wh_robot.pos_y][wh_robot.pos_x - 1].item_type == item::type::empty_space) {
			wh_robot.pos_x--;
			return;
		}

		for (int i = wh_robot.pos_x -1; i >= 0 ; i--) {
			if (items[wh_robot.pos_y][i].item_type == item::type::wall) {
				wall_pos_x = i;
				break;
			}
			else if (items[wh_robot.pos_y][i].item_type == item::type::empty_space) {
				free_space_pos_x = i;
				free_spaces++;
				break;
			}
		}
		if (free_spaces == 0) return;
		for (int i = free_space_pos_x; i < wh_robot.pos_x; i++) {
			this->move(i, wh_robot.pos_y, /**/ i + 1, wh_robot.pos_y);
		}
		wh_robot.pos_x--;
	}

	void try_move_up() {
		int wall_pos_y = -1;
		int free_space_pos_y;
		int free_spaces = 0;

		if (items[wh_robot.pos_y - 1][wh_robot.pos_x].item_type == item::type::empty_space) {
			wh_robot.pos_y--;
			return;
		}

		for (int i = wh_robot.pos_y - 1; i >= 0; i--) {
			if (items[i][wh_robot.pos_x].item_type == item::type::wall) {
				wall_pos_y = i;
				break;
			}
			else if (items[i][wh_robot.pos_x].item_type == item::type::empty_space) {
				free_space_pos_y = i;
				free_spaces++;
				break;
			}
		}
		if (free_spaces == 0) return;
		for (int i = free_space_pos_y; i < wh_robot.pos_y; i++) {
			this->move(wh_robot.pos_x, i /**/, wh_robot.pos_x, i + 1);
		}
		wh_robot.pos_y--;
	}

	void try_move_down() {
		int wall_pos_y = -1;
		int free_space_pos_y;
		int free_spaces = 0;

		if (items[wh_robot.pos_y + 1][wh_robot.pos_x].item_type == item::type::empty_space) {
			wh_robot.pos_y++;
			return;
		}

		for (int i = wh_robot.pos_y +1; i < items.size(); i++) {
			if (items[i][wh_robot.pos_x].item_type == item::type::wall) {
				wall_pos_y = i;
				break;
			}
			else if (items[i][wh_robot.pos_x].item_type == item::type::empty_space) {
				free_space_pos_y = i;
				free_spaces++;
				break;
			}
		}
		if (free_spaces == 0) return;
		for (int i = free_space_pos_y; i > wh_robot.pos_y; i--) {
			this->move(wh_robot.pos_x, i - 1 /**/, wh_robot.pos_x, i);
		}
		wh_robot.pos_y++;
	}

	
	/*
	Adjust for twice ToDO
	*/

	void try_move_up2() {
		int wall_pos_y = -1;
		int free_space_pos_y;
		int free_spaces = 0;

		if (items[wh_robot.pos_y - 1][wh_robot.pos_x].item_type == item::type::empty_space) {
			wh_robot.pos_y--;
			return;
		}

				

		wh_robot.pos_y--;
	}

	void try_move_down2() {
		int wall_pos_y = -1;
		int free_space_pos_y;
		int free_spaces = 0;

		if (items[wh_robot.pos_y + 1][wh_robot.pos_x].item_type == item::type::empty_space) {
			wh_robot.pos_y++;
			return;
		}

		wh_robot.pos_y++;
	}
};


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
    load_data("input_day15.txt", lines);
	warehouse wh(lines, false);
	int total = wh.move();
    write_sln("solution_day15_p1.txt", std::to_string(total)); 
}
void part_2() { 
    std::vector<std::string> lines;
    load_data("input_day15.txt", lines);
	warehouse wh(lines, true);
	int total = wh.move2();
    write_sln("solution_day15_p2.txt", std::to_string(total));
}
int main() {
    part_1();
    part_2();
    return 0;
}
