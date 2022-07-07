COMP_OPT = -Wall -Wextra -Wfatal-errors -std=c++11 #-O3

RP_Test.exe: RP_Driver.o RP_Greedy.o Router_placement_Data.o Random.o
	g++ -o RP_Test.exe RP_Driver.o RP_Greedy.o Router_placement_Data.o Random.o

RP_Data.o: Router_placement_Data.cc Router_placement_Data.hh
	g++ $(COMP_OPT) -c Router_placement_Data.cc RP_Data.o

RP_Greedy.o: RP_Greedy.cc RP_Greedy.hh Router_placement_Data.hh
	g++ $(OPTIONS) -c RP_Greedy.cc

Random.o: Random.cc Random.hh
	g++ $(OPTIONS) -c Random.cc

RP_Driver.o: RP_Driver.cc Router_placement_Data.hh
	g++ $(COMP_OPT) -c RP_Driver.cc

clean:
	rm -f Router_placement_Data.o RP_Greedy.o RP_Driver.o RP_Test.exe
