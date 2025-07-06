# C++ Makefile for HW5
#
# Replace the LastName-FirstName in the package section with
# your actual first and last name.

CXX=g++
CXXFLAGS=-Wall -Wextra -Wpedantic -Werror -Wfatal-errors -Winit-self -Wshadow -Wlogical-op -Wcomment -Wctor-dtor-privacy -Wold-style-cast -D_GLIBCXX_DEBUG -fno-diagnostics-show-option

all: Scheduler.cc Job.cc
	$(CXX) $(CXXFLAGS) -o Scheduler Scheduler.cc Job.cc

		
clean:
	rm -f Scheduler *.o *.zip
	
package:
	zip Vercauteren-Sky-HW5.zip README.txt Makefile Scheduler.cc Job.cc Job.h
