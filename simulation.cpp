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

		int k, pid, a_time, run_time;
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
    
    bool MM_add(int t, int k){
        //update ops
		return false;
    }
	void MM_remove(int k, int t) {


		//update free memory
	}
	void update_Memory_Map() {
		//when process comes allocate the memory

		/*
		map<int, list<int>>::iterator iter;
		iter = remove.find(a_time);

		if (iter == remove.end())
			remove.insert(a_time, k);
		else
			iter->second.push_back(k);
		*/
		//when process finished free the memory
	}
    void virtual_clock(){
        int t = 0;
        while(t <= 100000 || queue.size() != 0){// add more condidions
            //update 
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
	map<int, list<pair<bool, int>>> events; // time t, PID
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
    
    return 0;
}