all: main.cpp
	g++ main.cpp -o PicServe.exe -lboost_system -lboost_filesystem -lexiv2

clean:
	rm PicServe.exe
