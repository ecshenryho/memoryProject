#include<iostream>
#include<fstream>
#include<vector>
#include<list>
using namespace std;

class Simulation{
private:
    vector<int> memory_map;
    list<*Process> queue;
    vector<Process> process_list;
    list<pair<int, list<int>>> operations;
    int free_memory;
    int capacity;
    int page_size
public:
    Simulation(int mem_size, int size, int process_number){
        capcacity = mem_size;
        page_size = size *100;
    }
    bool read_file(const string& path){
        return false;
        add_process(Process(/* argus */));
    }
    void add_queue(Process program/*argus */){
        queue.push_back(*program);
        //check operations and add to operations
    }
    bool add_mem(const * Process, int t){
        //update ops
        return false
    }
    void remove_mem(*Process, int t){

    }
    //update free memory
    void update_Memory_Map(){
        //when process comes allocate the memory

        //when process finished free the memory
    }
    void virtual_clock(){
        int t = 0;
        while(t <= 100000 || queue.size() != 0){
            //update 
            t += 100;
        }
    }
    ostream& print_mem(int time);
    int turn_around_time();
    ~Simulation(){

    }



};




class Process{
private:
    /* data */
    int process_pid;
    int process_arrival_time;//time arrrive of a process
    vector<int> process_memory;
    int process_time_complete;//time completed of a process
    vector<int>page_number;//keep track of pages has been used
public:
    process(/* args */0){

    }
    ~process(){

    }
    //function sum process memory
    int sum_memory(){
        int sum=0;
        
        for(int i=0;i <process_memory.size();i++ ) {
            sum+=process_memory[i];
        }
    }
    return sum;
    //compute turn around time
};


int main(){
    int mem_size, page_size;
    cout << "Enter memory size: ";
    cin >> mem_size;
    cout << endl << "Enter page size: (1:100, 2:200, 3:400)";
    cin >> page_size;
    if (page_size < 1 || page_size > 3)
        cout << "dkhfkjhdshf" // DO EXCEPTION LATER;
    
    
    Simulation sim(mem_size, page_size);
    if (!sim.read_file("in1.txt"))
        cout << "dkhfkjhdshf" // DO EXCEPTION LATER;

    
    return 0;
}