Hello! Welcome to my HW5 submission:

### Note to the grader:
Thanks for your time! Im reading my code in review and two things jump out at me:
- I commented *everything* and its perhaps even overcommented. I would rather be safe than sorry, and I want to make your life easier if I can explain my thinking. 
- Theres a lot going on. I am hoping I didnt overengineer the project. I probably did, but hopefully the amount of over-production is just in the form of helpers etc and not in the actual scheduling logic. 

### Approach:
I started to build everything out using an array<int> to represent a process but quickly realized I would need to be consistently reading, adding, and changing this information, so I decided to build an object. 
The object "Job" represents an objecth, and is responsible for getting and setting its basic information, as well as tracking how long it has been on the CPU, how much of its qwuantum is left, etc. I think that choice made everything a lot easier to understand, But I am hoping the extra code doesnt make it harder to read.

### Files:
- Job.h
    The header for the class/object that represents a single process. 
- Job.cc 
    The onject that holds a process. Responsible for providing information to the main, as well as tracking, incrementing and decrementing counter and index values. 
- Scheduler.cc
    + Parses the file given in the argument into a data structure of Job objects
    + Uses FIFO , SJFP and Priority Scheduling algorithms to organize the list of tasks
    + counts the number of processes executed and tracks throughput, waiting time, and turnaround time
- Makefile
    For building the executable
- Readme
    this.