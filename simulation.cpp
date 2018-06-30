#include<isotream>
#include<fstream>
#include<vector>
using namespace std;

class Simulation{
private:
    /* data */
public:
    Simulation(/* args */){

    }
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
    process(/* args */){

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


    system("pause");
    return 0;
}