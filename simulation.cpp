#include<iostream>
#include<fstream>
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

	int pages_needed(int page_size) {
		int sum = 0;
		for (int i = 0; i < memory_chunks.size(); i++) {
			sum += memory_chunks[i] % page_size;
			if (memory_chunks[i] % page_size != 0) sum++;
		}
		return sum;
	}

	int turnaroundTime() {
		return completion_time - arrival_time;
	}

	int total_memory() { // might not need
		int sum = 0;
		for (int i = 0; i < memory_chunks.size(); i++) 
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

class Simulation{
public:
    Simulation(int mem_size, int size){
        capacity = mem_size;
		if (size == 1) page_size = 100;
		else if (size == 2) page_size = 200;
		else page_size = 400;
		free_pages = capacity / page_size;
		memory_map = vector<int>(-1, free_pages);
    }

	bool read_file(const string& path) {
		//if not open file
		return false;

		int k, pid, a_time, run_time; //all you need to do is read process info into these vars(except k and iter) and loop
		vector<int> memory_needed;
		map<int, list<pair<bool, int>>>::iterator iter;

		//while reading process from file...
		process_list.push_back(Process(pid, a_time, run_time, memory_needed));
		k = process_list.size() - 1; //Hash of PID
		iter = events.find(a_time);
		if (iter == events.end())
			events.insert(a_time, (true, k)); // when testing make sure this is correct
		else
			iter->second.push_back(pair<bool, int>(true, k));
    }
    
    void MM_add(int k){
		cout << "MM moves Process " << k << "to memory" << endl;
		size_t size = process_list[k].pages_needed(page_size);
		free_pages -= size;
		int i = 0;

		while (size > 0) {
			if (memory_map[i] == -1) {// if page is free else check next page
				memory_map[i] = k;
				size--;
			}
			i++;
		}

		print_queue();
		print_mem();
    }
	void MM_remove(int k) {
		cout << "Process " << k <<" completes" << endl;
		size_t size = process_list[k].memory_chunks.size();
		for (size_t i = 0; i < size; i++)
			memory_map[process_list[k].memory_chunks[i]] = -1;
		free_pages += size;
		print_mem();
	}
	void enqueue(int k) {
		cout << "Process" << k << " arrives" << endl;
		queue.push_back(k);
		print_queue();
	}

    void virtual_clock(){
        int t = 0;
        while(t <= 100000 || events.size() != 0){
			if (t == events.begin()->first) {//check if any events at time t
				cout << "t = " << t << ": ";
				while (events.begin()->second.size() != 0) { // run through all events
					pair<bool, int> action = events.begin()->second.front(); // pointer to list
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
					queue.pop_front();
				}
				iter++;
			}
            t += 100;
        }
    }

	void print_mem() {
		size_t mem_size = memory_map.size();
		int start;
		cout << "Memory Map:";
		for (size_t i = 0; i < mem_size; i++) {
			if (memory_map[i] != -1) {
				if (start != -1) {
					cout << (start + 1) * 100 << " - " << (i + 1) * 100 + 99 << ": Free frame(s)" << endl;
					start = -1;
				}
				else
					cout << (i + 1) * 100 << " - " << (i + 1) * 100 + 99 <<
					": Process " << memory_map[i] << ", Page" << i + 1 << endl;
			}
			else
				start = i;
		}
		cout << endl;
	}

	void print_queue() {
		size_t size = queue.size();
		cout << "Input Queue:[";
		for (size_t i = 0; i < size; i++)
			cout << queue[i] << " ";
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

int main(){
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