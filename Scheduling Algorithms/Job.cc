//Sky Vercauteren
// CS 370 colo state university
// HW 5

/* An object to hold a process object 
instead of an array of int and trying 
to remember what item in the array 
represented what information */

#include "Job.h"
#include <iostream>
#include <string>

using namespace std;

// a constructor for each job, initializes all the values. 
Job::Job(int index, int enter, int burstTime, int rank){
    //set the objects data to the given data from reading a file.
    id = index;
    entrance_time = enter;
    burst_time = burstTime;
    priority = rank;

    //start the necessary counters.
    waiting = 0;
    completion_time = 0; //the run time is the burst time before its decremented

    //other
    is_null = false;        //this process is not null, it can't be, we just initialized it.
}
//default constructor. we have no arguments, so the process is empty. used to represent idling cpu. running an empty process.
Job::Job(){ is_null = true; }

// getters do-ers and setters
int Job::get_id() const {return id;}                                        // getter for the id, just for debugging.
int Job::get_entrance_time() const {return entrance_time;}                  // getter for entrance time
int Job::get_burst_time() const {return burst_time;}                        // getter for burst time
int Job::get_priority() const {return priority;}                            // getter for priority
int Job::get_waiting() const {return waiting;}                              // getter for waiting
int Job::get_turnaround() const {return completion_time - entrance_time;}   // getter for turnaround, will always be a sum of running and waiting
bool Job::get_is_null() const {return is_null;}                             // returns if the process == "nothing" "Null" "empty" etc. 

void Job::set_completion_time(int clock) {
    completion_time = clock;
}

void Job::decrease_burst() {burst_time -= 1;}           // decrease burst time by one unit
void Job::increase_waiting() {waiting +=1;}             // increase waiting time by one unit
                                                 
void Job::clear(){ is_null = true;}                       // results in an empty job. represents idling. e.g. when the CPU is idle, the current job is "nothing".