//Sky Vercauteren
// CS 370 colo state university
// HW 5

/* An object to hold a process object 
instead of an array of int and trying 
to remember what item in the array 
represented what information */

#include <fstream>
#include <iostream>
#include <string>
#include <array>
#include <deque>
#include <tuple>
#include <algorithm> // for std::sort()
#include <cmath>    // for static_cast<int> so it accepts my loops.
#include <iomanip> // for cout<<setprecision(3)
#include "Job.h"

using namespace std;

/*-------------------------------*/
/* --- Function Declarations --- */
/*-------------------------------*/
deque<Job> readFile(const string& filename);                                // reads a csv file into data structure

tuple<deque<Job>,int> scheduler(deque<Job> processes,string algorithm);     // the main logic for scheduling jobs.

void report(tuple<deque<Job>,int> data);                                    // used to collect totals, format, and print output.

deque<Job> tieBreak(string algorithm, deque<Job>);                          // helper for sorting a deque and breaking ties.


/*----------------------*/
/* --- Main Function ---*/
/*----------------------*/

// 1. Check args
// 2. Read file
// - - Set quantum
// 3. call FIFO
// - - Report
// 4. Call SJFP
// - - Report
// 5. call Priority
// - - Report
int main(int argc, char* argv[])
{
    // 1. first, ensure that the program only runs when given a single argument
    if(argc != 2)
    {
        cout<<"Incorrect number of arguments."<<endl;
        return -1;
    }

    // 2. read the file
    string filename = argv[1];
    //int quantum = 0;

    // "processes" is the variable that holds all the jobs bwefore they arrive. 
    // It could be seen as a file buffer, holding simply the formatted contents of the input file.
    // - - note: this call will also set the value of "quantum" by reference, to be 1+ the mean burst time. 
    deque<Job> processes = readFile(filename);

    // 3. Call FIFO and report
    tuple<deque<Job>,int> fifo_data = scheduler(processes, "FIFO");     // stores the queue of finished processes as well as the cpu_clock used in the FIFO process.
    cout<<"--- FCFS ---"<<endl;
    report(fifo_data);                               // uses the data from finished processes and cpu_clock from the FIFO process. 

    // 4. Call SJFP and report
    tuple<deque<Job>,int> sjfp_data = scheduler(processes, "SJFP");     // stores the queue of finished processes as well as the cpu_clock used in the SJFP process.
    cout<<"--- SJFP ---"<<endl;
    report(sjfp_data);                               // uses the data from finished processes and cpu_clock from the SJFP process. 

    // 5. Call Priority and report
    tuple<deque<Job>,int> priority_data = scheduler(processes, "Priority");     // stores the queue of finished processes as well as the cpu_clock used in the Priority process.
    cout<<"--- Priority ---"<<endl;
    report(priority_data);                               // uses the data from finished processes and cpu_clock from the Priority process. 

    return 0;   
}

/*------------------------------*/
/* --- Function Definitions --- */
/*------------------------------*/

//Read File: 
// - helper to read in a file and return a populated data structure
deque<Job> readFile(const string& filename) {

    // This is the queue to hold each line of the file. 
    deque<Job> jobs;

    //open the file
    ifstream file(filename);

    //temporary space for reading the file into the queue
    int value;
    array<int, 4> job;
    int i = 0; 

    //since we are reading the file for the first time, we might as well count the burst time to determine the quantum
    //int total_burst = 0; 

    // while you can read the next character, 
    while (file >> value) {

        job[i] = value;       // read the first value
        i++;                 // note: this line is necessary after we store i=3 to trigger the following condition 

        //if we have stored all 4 variables:
        // -- construct a new process object, store it, 
        // -- and reset for the next line. 
        if (i == 4) {
            jobs.push_back(Job(job[0], job[1], job[2], job[3]));
            i = 0;
        }
        
        if (file.peek() == ',') file.ignore(); // skip comma
    }
    return jobs;
}

//Scheduler: 
// - The main logic for listening for jobs, scheduling jobs, and terminating jobs.
// - Returns the finished processes Queue, and the total runtime, needed for formatted output.
tuple<deque<Job>,int> scheduler(deque<Job> processes, string algorithm){
    
    // initial size of incoming jobs, used to see if there are incomplete jobs.
    size_t initial_size = processes.size();
    
    // Vars for scheduling:
    deque<Job> readyQ;       // ready queue - holds processes that have enetered the system but not run.
    Job runningJob;           // This represents the CPU. If a Job is here, it means it is running. 
    deque<Job> doneQ;          // finished queue - holds processes that have finished running. 
                                //  -- when a process is done, its burst time will be 0, and its waiting time will be nonzero.

    int cpu_clock = 0;        // the main counter that represents total elapsed time.

    int mostImportantLevel = 1000; // I know the priority levels only go up to 50, but this is easier to see in the code. 

    // The main loop - 
    // while there are incompleted jobs:
    // 1. listen for incoming jobs and add them to the ready queue
    // 2. if the cpu is free, schedule the next job from the ready queue 
    // 3. do work, 
    // 4. increment waiting jobs, 
    // 5. and increment the cpu clock. 
    while(doneQ.size() < initial_size)
    {
        // 1. get the next incoming job to check if it can start yet.
        if(!processes.empty())
        {
            // Account for ties by grabbing *all* the process that arrive at a given time.
            // - - rewrote this from, previous versions, which was sequencial and only grabbed one at time.
            deque<Job> incomingJobs;
            for(int i = static_cast<int>(processes.size())-1; i>=0; --i)     // iterating backwards to avoid shifting the indices by removing/erasing.
            { 
                // grab the arriving process.
                if(processes[i].get_entrance_time() == cpu_clock)
                {
                    //add it to list of potential ties and remocve it from the file buffer
                    incomingJobs.push_back(processes[i]);
                    processes.erase(processes.begin() + i);
                }
            }

            // incomingJobs now represents *all* the jobs that have arrived this cycle. i.e. all the *TIED* processes.
            // the order that we put these into the ready Q will determine how we break the ties.
            // - - considering writing a helper function for breaking ties, maybe I can just use sort() on the deque.
            if(!incomingJobs.empty())
            {
                incomingJobs = tieBreak(algorithm, incomingJobs);
            }
            
            // - PREEMPTION!! -
            // preemption should only happen if a job is running, otherwise proceed with original scheduling
            if(!runningJob.get_is_null()){
                if(algorithm == "SJFP")
                {
                    // check to see if the preemption condition is met, exit once it has been so that only a single preemtion occurs
                    for(int j = static_cast<int>(incomingJobs.size())-1; j>=0; --j)
                    {
                        if(incomingJobs[j].get_burst_time() < runningJob.get_burst_time())  // preemption condition is met
                        {
                            //push current process back to waiting and clear the cpu
                            readyQ.push_back(runningJob);
                            runningJob.clear();
                            // skip the ready Q and run process
                            runningJob = incomingJobs[j];
                            incomingJobs.erase(incomingJobs.begin() + j);
                            //exit loop.
                            break;
                        }
                    }
                }
                else if(algorithm == "Priority")
                {
                    // check to see if the preemption condition is met, exit once it has been so that only a single preemtion occurs
                    for(int j = static_cast<int>(incomingJobs.size())-1; j>=0; --j)
                    {
                        if(incomingJobs[j].get_priority() < mostImportantLevel)     // preemption condition is met
                        {
                            // push current process back to waiting and clear the cpu
                            readyQ.push_back(runningJob);
                            runningJob.clear();
                            // set new priority
                            mostImportantLevel = incomingJobs[j].get_priority();
                            // skip the ready Q and run process
                            runningJob = incomingJobs[j];
                            incomingJobs.erase(incomingJobs.begin() + j);
                            //exit loop.
                            break;
                        }
                    }
                }     
            }

            // Add the rest of the incoming job(s) to the ready que
            // - - we need to go forward here to push the order in correctly.
            for(int m = 0; m <static_cast<int>(incomingJobs.size()); m++)
            {
                readyQ.push_back(incomingJobs[m]);
            }
        }

        // it's possible that there is no incoming job AND no current job, (i.e. a true idle)
        if(runningJob.get_is_null() && readyQ.size() < 1)
        {
            // increment clock and go to next loop.
            cpu_clock++; 
            continue;   
        }

        // 2. determine if a process is running 
        if(runningJob.get_is_null())
        {
            // if there is no job on the processor, we can schedule one. 
            // THIS IS WHERE A JOB IS SCHEDULED ACCORDING TO THE DESIRED ALGORITHM

              if(algorithm == "FIFO")
            { 
                if (!readyQ.empty()) {
                    runningJob = readyQ.front(); // Take the first job. send it to the cpu.
                    readyQ.pop_front();          // Remove it from the front
                } else {
                    cout<<"Execution shouldn't ever get here."<<endl;
                }
            }

            // -- SJF -- //
            // note: the preemption is done further below as the quantum expires.
            if(algorithm == "SJFP")
            {
                // find the shortest job.
                int shortest = 1000;        //compare each burst time
                int shortestIndex = -1;     //hold the index of the current shortest winner
                for(size_t i = 0; i<readyQ.size(); i++)
                {
                    int burst = readyQ[i].get_burst_time();
                    if( burst < shortest)
                    {
                        shortest = burst;
                        shortestIndex = i;
                    }  
                    //In the event of a tie do nothing, and keep the original job - trust the output of the above tieBreak()
                }
                // send it to the cpu and remove it from ready queue
                if(shortestIndex != -1)
                {
                    runningJob = readyQ[shortestIndex];
                    readyQ.erase(readyQ.begin() + shortestIndex);
                }
                else{
                    cout<<"Execution shouldn't ever get here, it it does check the logic in the SJFP scheduling"<<endl;
                }
            }

            // -- PRIORITY -- //
            // - Note: the preemption is done above, as new processes enter
            // - this will only pick from the ready Q when a process finishes.
            if(algorithm == "Priority")
            {
                // find the most important job.
                int importantIndex = -1;        //hold the index of the current shortest winner
                int importantLevel = 1000;
                for(size_t i = 0; i<readyQ.size(); i++)
                {
                    int level = readyQ[i].get_priority();
                    if( level < importantLevel)
                    {
                        importantLevel = level;
                        importantIndex = i;
                    }
                    //otherwise, keep the currently selected Job
                }   

                // if it appropriately selected a process
                if(importantIndex != -1)
                {
                    // send it to the cpu and remove it from the ready queue
                    runningJob = readyQ[importantIndex];
                    mostImportantLevel = importantLevel;
                    readyQ.erase(readyQ.begin() + importantIndex);
                }else{
                    cout<<"Execution shouldn't ever get here, it it does check the logic in the priority scheduling"<<endl;
                }
            }
        }
        // 3. if there *is* a process running, we can eat a unit of burst time.
        runningJob.decrease_burst();

        // if the job just used its last burst cycle, we can terminate it
        if(runningJob.get_burst_time() == 0)
        {
            runningJob.set_completion_time(cpu_clock + 1);  // should it be *this* clock cycle or the next one? 
            doneQ.push_back(runningJob);                    // add it to the list of completed jobs
            runningJob.clear();                             // remove the job from the CPU so the next one can be scheduled.
                                                            // -- The current runningJob is now "Nothing"
        }

        // 4. every other process in the ready queue needs to increment their waiting time.
        for(size_t r =0; r< readyQ.size(); r++){
            readyQ[r].increase_waiting();
        }

        // 5. Increment global clock
        cpu_clock++;
    }
    return make_tuple(doneQ, cpu_clock);
}

//Report: 
// - A function to iterate over a list of processes and record averages, format the data, and print it to the console.
void report(tuple<deque<Job>,int> scheduler_data)
{
    // data extracted from scheduler
    deque<Job> doneQ = get<0>(scheduler_data);
    int cpu_clock = get<1>(scheduler_data);

    // variables to hold totals
    int total_waiting = 0;
    int total_turnaround = 0;

    // iterate the processes and add the totals
    for(Job job : doneQ)
    {
        total_waiting += job.get_waiting();
        total_turnaround += job.get_turnaround();
    }
    //find the average waiting and turnaround times.
    float avg_waiting = (1.0f * total_waiting) / doneQ.size();
    float avg_turnaround = (1.0f * total_turnaround) / doneQ.size();

    // the number of jobs per unit of time
    float throughput = doneQ.size() / (1.0f *cpu_clock);

    //Formatted output
    cout<<"Average Turnaround Time: "<< fixed << setprecision(3) << avg_turnaround <<endl;
    cout<<"Average Waiting Time: "<< fixed << setprecision(3) << avg_waiting <<endl;
    cout<<"Throughput: "<< fixed << setprecision(3) << throughput <<endl;
    cout<<endl;
}

// helper for sorting a deque and breaking ties based on the current algorithm.
// uses this struct and the following method

// Ok this is probably significantly over-engineered, but searching for a consice way to sort the deque led me to overloaded comparator structs. 
// so full disclosure, this is all just sort of for fun, and a good refresher on overloading and structs.
// the reason I am not just making a comparison function, is that there is no way to pass a comparison function AND a 3rd argument ("algorithm") to the sort function. 
// I could just make my own sort function, but this is more fun. 

struct CompareJobs{
    string algorithm;
    // pass the algorithm through the constuctor     
    CompareJobs(const string& alg){ algorithm = alg;}
    // overload the function call operator -- basically this says, "you can call this struct as a function that takes two arguments"
    // in our case we will use this to compare two objects. - using const references by convention to avoid coppying and modifying the values to be compared.
    bool operator()(const Job& a, const Job& b)
    {
        // For FIFO: by ID
        if (algorithm == "FIFO") {
            return a.get_id() < b.get_id();
        }
        // For SJFP: by shortest burst
        if (algorithm == "SJFP") {
            return a.get_burst_time() < b.get_burst_time();
        }
        // For Priority: by lowest priority value (higher priority) first
        if (algorithm == "Priority") {
            return a.get_priority() < b.get_priority();
        }
        return false; // should never get here :3
    }
};

// method using the above struct to break ties.
deque<Job> tieBreak(string alg, deque<Job> list)
{
    //1. make a new comparitor based on the algorithm were tie breaking 
    CompareJobs comp(alg);

    //2. sort the list!
    sort(list.begin(), list.end(), comp);

    //3. return the sorted list.
    return list;
}