//Sky Vercauteren
// CS 370 colo state university
// HW 5

/* An object to hold a process object 
instead of an array of int and trying 
to remember what item in the array 
represented what information */

#ifndef JOB_H
#define JOB_H

#include <iostream>
#include <string>

using namespace std;

//The object that represents a process
class Job {

    private:
        // csv data
        int id;                 // PID - the first number provided in the csv file
        int entrance_time;      // Entrance Time -the second number provided in the csv file
        int burst_time;         // Burst Cycle Time - the third number provided in the csv
        int priority;           // Priority rating - the last number provided in the csv

        // counter data
        int waiting;            // the time spent in the ready queue, after entering but before running
        int completion_time;    // the value of the clock whebn a process finishes

        // other
        bool is_null;       // used to track if the process is "empty" or "nothing" - this will be true when the CPU is idling. 

    public:
        // a constructor for each job, initializes all the values. 
        Job(int index, int enter, int burstTime, int rank);
        //default constructor. we have no arguments, so the process is empty. used to represent idling cpu. running an empty process.
        Job();

        // getters
        int get_id() const;               // getter for the id, just for debugging.
        int get_entrance_time() const;    // getter for entrance time
        int get_priority() const;         // getter for priority
        int get_burst_time() const;       // getter for burst time
        int get_waiting() const;          // getter for waiting
        int get_turnaround() const;       // getter for turnaround, will always be a sum of running and waiting
        bool get_is_null() const;         // returns if the process == "nothing" "Null" "empty" etc. 

        // counters
        void decrease_burst();       // decrease burst time by one unit
        void increase_waiting();     // increase waiting time by one unit

        // other do-ers and setters
        void set_completion_time(int clock);    // the time at cpu_clock when a process completes.
        void clear();                           // results in an empty job. represents idling. e.g. when the CPU is idle, the current job is "nothing".
};

#endif