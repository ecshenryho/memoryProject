#include<iostream>
#include<fstream>
#include <string>
#include<vector>
#include<list>
#include <map>
using namespace std;

class Process {
public:
	Process(int pid_, int a_time, int run_time_,
		const vector<int>& memory_needed) {
		pid = pid_;
		arrival_time = a_time;
		run_time = run_time_;
		memory_chunks = memory_needed;
	}

	int pages_needed(int page_size) const {
		int sum = 0;
		for (size_t i = 0; i < memory_chunks.size(); i++) {
			sum += memory_chunks[i] / page_size;
			if (memory_chunks[i] % page_size != 0) sum++;
		}
		return sum;
	}

	int find_page(int m) {
		size_t size = pages_used.size();
		for(size_t i = 0; i < size; i++)
			if (pages_used[i] == m)
				return i + 1;
		return -1;
	}

	int turnaroundTime() const {
		return completion_time - arrival_time;
	}

	int total_memory() { // might not need
		int sum = 0;
		for (size_t i = 0; i < memory_chunks.size(); i++)
			sum += memory_chunks[i];
		return sum;
	}

	~Process() {}
private:
	int pid,
		arrival_time,
		run_time,
		completion_time;
	vector<int> memory_chunks;
	vector<int>pages_used;
	friend class Simulation;
};

class Simulation {
public:
	Simulation(int mem_size, int size) {
		capacity = mem_size;
		if (size == 1) page_size = 100;
		else if (size == 2) page_size = 200;
		else page_size = 400;
		free_pages = capacity / page_size;
		memory_map = vector<int>(free_pages, -1);

	}

	bool read_file(const string& path) {
		ifstream file(path);
		if (!file.is_open()) {
			cout << "Can not open the file: " << path << endl;
			return false;
		}

		int k, pid, a_time, run_time, count, n, n_piece, c;
		

		file >> count;
		for (int i = 0; i < count; i++) {
			file >> pid;
			file >> a_time;
			file >> run_time;
			file >> n_piece;
			vector<int> memory_needed;
			c = 0;
			while (c < n_piece) {
				file >> n;
				memory_needed.push_back(n);
				c++;
			}

			process_list.push_back(Process(pid, a_time, run_time, memory_needed));
			k = process_list.size() - 1; //Hash of PID
			pair<bool, int> temp(true, k);
			list<pair<bool, int>> a = { temp };

			pair<map<int, list<pair<bool, int>>>::iterator, bool> mem
				= events.insert(pair<int, list<pair<bool, int>>>(a_time, a));
			if (!mem.second) // returns false if eleme already excists
				mem.first->second.push_back(temp);
		}

		file.close();
		file.clear();
		return true;
	}

	void MM_add(int k) {
		cout << "MM moves Process " << k + 1 << " to memory" << endl;
		size_t size = process_list[k].pages_needed(page_size);
		free_pages -= size;
		int i = 0;

		while (size > 0) {
			if (memory_map[i] == -1) {// if page is free else check next page
				memory_map[i] = k;
				process_list[k].pages_used.push_back(i);
				size--;
			}
			i++;
		}

		//add to remove queue

	}
	void MM_remove(int k) {
		cout << "Process " << k+1 << " completes" << endl;
		size_t size = process_list[k].pages_used.size();
		for (size_t i = 0; i < size; i++)
			memory_map[process_list[k].pages_used[i]] = -1;
		free_pages += size;
		print_mem();
	}
	void enqueue(int k) {
		cout << "Process " << k+1 << " arrives" << endl;
		queue.push_back(k);
		print_queue();
	}

	void virtual_clock() {
		int t = 0;
		while (t <= 100000 && events.size() != 0) {
			if (t == events.begin()->first) {//check if any events at time t // bug at 1700
				cout << "t = " << t << ": ";
				while (events.begin()->second.size() != 0) { // run through all events
					pair<bool, int> action = events.begin()->second.front(); // pointer to first elem in list
					if (action.first)  // if true, enqueue
						enqueue(action.second);
					else //if false terminate process
						MM_remove(action.second);
					events.begin()->second.pop_front();// iterate, check if correct
				}
				events.erase(events.begin());
			}
			list<int>::iterator iter = queue.begin(); // check queue
			while (iter != queue.end()) {
				if (process_list[*iter].pages_needed(page_size) <= free_pages) { //if enough space add to mem, if not check next
					MM_add(*iter);
					++iter;
					queue.pop_front();
					print_queue();
					print_mem();
				}
				else
				++iter;
			}
			t += 100;
		}
	}

	void print_mem() {
		size_t mem_size = memory_map.size();
		int start, tail;
		cout << "Memory Map:" << endl;
		for (size_t i = 0; i < mem_size; i++) {
			if (memory_map[i] != -1) {
				cout << i * 100 << " - " << i * 100 + 99 <<
					": Process " << memory_map[i] + 1 << ", Page " << 
					process_list[memory_map[i]].find_page(i) << endl;
			}
			else { // if page is empty loop til we find a filled page
				start = i;
				tail = i;
				while (i < mem_size && memory_map[i] == -1) {
					tail = i;
					i++;
				}
				i--;
				cout << (start) * 100 << " - " << (tail) * 100 + 99 << ": Free frame(s)" << endl;
			}
		}
		cout << endl;
	}

	void print_queue() {
		size_t size = queue.size();
		list<int>::const_iterator iter = queue.cbegin();
		list<int>::const_iterator iter_end = queue.cend();

		cout << "Input Queue:[ ";
		while (iter != iter_end) {
			cout << *iter + 1 << " ";
			iter++;
		}
		cout << "]" << endl;
	}

	double turn_around_time() {
		double sum = 0;
		size_t size = process_list.size();
		for (size_t i = 0; i < size; i++) {
			sum += process_list[i].turnaroundTime();
		}
		return sum / size;
	}

	~Simulation() {}

private:
	vector<int> memory_map; //virtual pages
	list<int> queue; // processes index in process_list
	vector<Process> process_list;
	map<int, list<pair<bool, int>>> events; // time , TRUE = ADD / FALSE = remove, PID
	int free_pages;
	int capacity;
	int page_size;
};

int main() {
	int mem_size, page_size;
	cout << "Enter memory size: ";
	cin >> mem_size;
	cout << endl << "Enter page size: (1:100, 2:200, 3:400)";
	cin >> page_size;
	if (page_size < 1 || page_size > 3)
		cout << "Out of bounds"; // DO EXCEPTION LATER;

	Simulation sim(mem_size, page_size);
	if (!sim.read_file("in1.txt"))
		cout << "Cant read file"; // DO EXCEPTION LATER;
	sim.virtual_clock();
	sim.turn_around_time();

	return 0;
}
