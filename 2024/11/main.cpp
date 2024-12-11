#include <fstream>
#include <iostream>
#include <regex>
#include <vector>
#include <sstream>

#include <map>
#include <locale>

struct Node {
    unsigned long long data;
    Node* next;
};

// Define the linked list class
class LinkedList {
    // Pointer to the first node in the list
    Node* head;
    Node* current_iteration_node;

public:
    // Constructor initializes head to NULL
    LinkedList() : head(NULL) {}
    unsigned long long size = 0;

	bool move_next() {
        current_iteration_node = current_iteration_node->next;
        return current_iteration_node != nullptr;
	}

    ~LinkedList() {
		while(true) {
            if (head == nullptr) return;
            auto item_to_delete = head;
            auto next_head = head->next;
            delete head;
            head = next_head;
		}
	}

    Node* current_node() const { return current_iteration_node; }
    void reset_current_node() { current_iteration_node = head; }
    void set_current_node(Node* n) { current_iteration_node = n; }

    Node* at(int position) {
        Node* n = head;
        if (position == 0) return n;
        while(position > 0) {
            n = n->next;
            position--;
        }
        return n;
    }

    

    void set(int position, int val) {
        Node* n = head;
        if (position == 0) { head->data = val; return;  }
        while (position > 0) {
            head = head->next;
            position--;
        }
        head->data = val;
    }

    // Function to Insert a new node at the beginning of the list
    void insertAtBeginning(int value) {
        Node* newNode = new Node();
        newNode->data = value;
        newNode->next = head;
        head = newNode;
        size++;
    }

    // Function Insert a new node at the end of the list
    void insertAtEnd(int value) {
        Node* newNode = new Node();
        newNode->data = value;
        newNode->next = NULL;

        // If the list is empty, update the head to the new node
        if (!head) {
            head = newNode;
            size = 1;
            return;
        }

        // Traverse to the last node
        Node* temp = head;
        while (temp->next) {
            temp = temp->next;
        }

        // Update the last node's next to the new node
        temp->next = newNode;
        size++;
    }

    // Function to Insert a new node at a specific position in the list
    void insertAtPosition(int value, int position) {
        if (position < 1) {
            return;
        }
		
        if (position == 1) {
            insertAtBeginning(value);
            return;
        }
        Node* newNode = new Node();
        newNode->data = value;

        // Traverse to the node before the desired position
        Node* temp = head;
        for (int i = 1; i < position - 1 && temp; ++i) {
            temp = temp->next;
        }

        // If the position is out of range, print an error message
        if (!temp) {
            delete newNode;
            return;
        }

        // Insert the new node at the desired position
        newNode->next = temp->next;
        temp->next = newNode;
        size++;
    }

    // Function to Delete the first node of the list
    void deleteFromBeginning() {
        if (!head) {
            return;
        }

        Node* temp = head;
        head = head->next;
        delete temp;
        size--;
    }

    // Function to Delete the last node of the list
    void deleteFromEnd() {
        if (!head) {
            size = 0;
            return;
        }

        if (!head->next) {
            delete head;
            head = NULL;
            size = 0;
            return;
        }

        // Traverse to the second-to-last node
        Node* temp = head;
        while (temp->next->next) {
            temp = temp->next;
        }

        //  Delete the last node
        delete temp->next;
        // Set the second-to-last node's next to NULL
        temp->next = NULL;
        size--;
    }

    // Function to Delete a node at a specific position in the list
    void deleteFromPosition(int position) {
        if (position < 1) {
            return;
        }

        if (position == 1) {
            deleteFromBeginning();
            return;
        }

        Node* temp = head;
        for (int i = 1; i < position - 1 && temp; ++i) {
            temp = temp->next;
        }

        if (!temp || !temp->next) {
            return;
        }
        // Save the node to be deleted
        Node* nodeToDelete = temp->next;
        // Update the next pointer
        temp->next = temp->next->next;
        // Delete the node
        delete nodeToDelete;
        size--;
    }

    void display() {
        if (!head) {
            std::cout << "List is empty." << std::endl;
            return;
        }

        Node* temp = head;
        while (temp) {
	        std::cout << temp->data << " -> ";
            temp = temp->next;
        }
        std::cout << "NULL" << std::endl;
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

struct stone_line {

	std::vector<std::string> stones_as_string;

    int numlen(unsigned long long num)  {
        return num < 10 ? 1 : static_cast<int>(log10(num) + 1);
    }

    void halve(unsigned long long input, unsigned long long&left, unsigned long long& right) {
        unsigned long long input_top = input;
        unsigned long long input_bot = 0;
        unsigned long long last_num = input;
        int len = numlen(input);
        int half_top = (len / 2) - 1;
        int half_bottom = (len / 2) - 1;
        while(half_top >= 0) {
            input_top = input_top / 10;
        	half_top--;
        }
        unsigned long long idx = 1;
        while (half_bottom >= 0) {
            last_num = last_num % 10;
            last_num = last_num * idx;
            input_bot = input_bot + last_num;
        	half_bottom--;
            input = input / 10;
            idx *= 10;
            last_num = input;
        }
        left = input_top;
        right = input_bot;
    }

    unsigned long long blink_faster(int amount) {
        LinkedList ll;
        for (const auto& i : stones_as_string) {
            ll.insertAtEnd(atoi(i.c_str()));
        }
        for (int blink = 0; blink < amount; blink++) {
            //ll.display();
            ll.reset_current_node();
        	while (ll.current_node()) {
                bool any_rule_applied = false;
                auto node = ll.current_node();
                if (node->data == 0) {
                    node->data = 1;
                	any_rule_applied = true;
                }
                if (numlen(node->data) % 2 == 0) {
                    any_rule_applied = true;

                    unsigned long long first_num = 0;
                    unsigned long long second_num = 0;
                    halve(node->data, first_num, second_num);
                    node->data = first_num;
                    Node* next = new Node;
                    next->data = second_num;
                    Node* after = node->next;
                    node->next = next;
                    next->next = after;
                    ll.size++;
                    ll.set_current_node(next);
                }

                if (!any_rule_applied) {
                    node->data = node->data * 2024;
                }
                ll.move_next();
            }
        }
        return ll.size;
	}

    unsigned long long  blink_many_times(int amount) {
        unsigned long long total = 0;
        std::map<unsigned long long, unsigned long long> map;
        for(const auto& stone : this->stones_as_string) {
            map.insert({ atoll(stone.c_str()), 1 });
	    }
        return blink_ultra_fast(amount, map);
    }

    void print_list(std::vector<unsigned long long>& tail) {
	    for(auto x : tail) {
            std::cout << x << ", ";
	    }
        std::cout << "\n";
    }

    std::vector<unsigned long long> blink(unsigned long long stone) {
        if (stone == 0) return { 1 };
        if(numlen(stone) % 2 == 0) {
            unsigned long long first_num = 0;
            unsigned long long second_num = 0;
            halve(stone, first_num, second_num);
            return { first_num, second_num };
        }
        return { stone * 2024 };
    }

    unsigned long long blink_ultra_fast(int blinks, std::map<unsigned long long, unsigned long long> map) {
        for (int i = 0; i < blinks; i++) {
            std::map<unsigned long long, unsigned long long> newStoneMap;
            for (const auto& kvp : map) {
                auto stone = kvp.first;
                auto count = kvp.second;
                auto blink_map = blink(stone);
                for (const auto& ns : blink_map) {
                    newStoneMap[ns] = newStoneMap[ns] + count;
                }
            }
            map = newStoneMap;
        }

        unsigned long long total = 0;
        for (const auto& kvp : map) {
            total += kvp.second;
        }
        return total;
    }
};



void load_data(const std::string& file, stone_line& stones) {
	std::ifstream ifs(file);
	if (!ifs.is_open()) { return; }
	std::string line;
	int row = 0;
	std::getline(ifs, line);
	auto stone_line = split(line, " ");
	stones.stones_as_string = stone_line;
}

void write_sln(const std::string& name, const std::string& sln) {
	std::ofstream ofs(name, std::ios::trunc);
	ofs << sln;
}

void part_1() {
	unsigned long long target = 0;
	stone_line stones;
	load_data("input_day11.txt", stones);
	target = stones.blink_faster(25);
	write_sln("solution_day11_p1.txt", std::to_string(target));
}

void part_2() {
	unsigned long long target = 0;
	stone_line stones;
	load_data("input_day11.txt", stones);
    target = stones.blink_many_times(75);
	write_sln("solution_day11_p2.txt", std::to_string(target));
}

int main() {
	part_1();
	part_2();
	return 0;
}
