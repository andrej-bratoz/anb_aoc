#include <fstream>
#include <iostream>
#include <regex>
#include <vector>
#include <limits>


struct matrix {
	void add_row(const std::string& row) { _rows.push_back(row); }
	bool has_obstacle(const int x, const int y) const { return _rows[y][x] == '#' || _rows[y][x] == 'O'; }
	void step(const int x, const int y) {
		if (_rows[y][x] != 'X') {
			_rows[y][x] = 'X';
			total_steps++;
		}
	}
	void set(const int x, const int y) {
		_rows[y][x] = 'O';
	}
	void reset(const int x, const int y) {
		if (_rows[y][x] == 'O') {
			_rows[y][x] = '.';
		} 
	}

	void print() const {
		for(const auto& line : _rows) {
			std::cout << line << "\n";
		}
	}
	std::vector<std::string> _rows;
	int total_steps = 1;
};

struct guard {
	enum class direction {
		north, east, south, west
	};
	int x, orig_x;
	int y, orig_y;
	int total_steps = 1;
	direction _direction = direction::north;
	long long max_steps;
	matrix _matrix;
	matrix _orig_mx;
	//
	guard(int _x, int _y, const matrix& mx) : x(_x), orig_x(x), y(_y), orig_y(y), _matrix(mx), _orig_mx(mx) { max_steps = mx._rows.size() * mx._rows[0].size(); }
	bool step_next() {
		switch(_direction) {
			case direction::north: {
				if (y - 1 < 0) return false; // we have stepped out of the map
				if (_matrix.has_obstacle(x, y - 1)) _direction = direction::east;
				else {
					y = y - 1;
					total_steps++;
				}
				break;
			}
			case direction::east: {
				if (x + 1 == _matrix._rows[0].length()) return false; // we have stepped out of the map
				if (_matrix.has_obstacle(x + 1, y)) _direction = direction::south;
				else {
					x = x + 1;
					total_steps++;
				}
				break;
			}
			case direction::south: {
				if (y + 1 == _matrix._rows.size()) return false; // we have stepped out of the map
				if (_matrix.has_obstacle(x, y + 1)) _direction = direction::west;
				else {
					y = y + 1;
					total_steps++;
				}
				break;
			}
			case direction::west: {
				if (x - 1 < 0) return false; // we have stepped out of the map
				if (_matrix.has_obstacle(x - 1, y)) _direction = direction::north;
				else {
					x = x - 1;
					total_steps++;
				}
				break;
			}
		}
		//_matrix.step(x, y);
		return true;
	}

	int try_infinite_loops() {
	}
};

void load_data(const std::string& file, matrix& mx, int& x, int& y) {
	std::ifstream ifs(file);
	if (!ifs.is_open()) { return; }
	std::string line;
	int row = 0;
	while (std::getline(ifs, line)) {
		auto guard_pos = line.find('^');
		if(guard_pos != std::string::npos) {
			y = row;
			x = static_cast<int>(guard_pos);
		}
		mx.add_row(line);
		row++;
	}
}

void part_1() {
	int x, y;
	matrix mx;
	load_data("input_day6.txt", mx, x, y);
	guard g(x, y, mx);
	while(g.step_next()) {}
	int sln = g._matrix.total_steps + 1; // +1 for start
}

void part_2() {
	int x, y;
	matrix mx;
	load_data("input_day6.txt", mx, x, y);
	guard g(x, y, mx);
	g.try_infinite_loops();
}

int main() {
	part_1();
	part_2();
	return 0;
}