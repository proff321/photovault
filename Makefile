all: main.o picSorter.o
	g++ -o PhotoVault main.o picSorter.o -lboost_system -lboost_filesystem -lexiv2 -lboost_program_options

main.o: main.cpp picSorter.h
	g++ -c main.cpp

picSorter.o: picSorter.h picSorter.cpp
	g++ -c picSorter.cpp

clean:
	rm -f PhotoVault picSorter.o main.o
