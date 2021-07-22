CXX = g++
INC_DIR = ../graph/
ifndef FF_ROOT 
FF_ROOT = ${HOME}/fastflow
endif
CXXFLAGS = -std=c++17 -pthread -g
IFLAG = -c -Wall -I$(INC_DIR) -I$(FF_ROOT) -fpermissive
OFLAGS = -O3
MUL = -DMUL


BFS_ff: BFS_ff.o graphfile_manager.o Node.o  
	$(CXX) $(CXXFLAGS) $(OFLAGS) -o BFS_ff BFS_ff.o graphfile_manager.o Node.o

BFS_ff.o: BFS_ff.cpp graphfile_manager.o Node.o
	$(CXX) $(CXXFLAGS) $(OFLAGS) $(IFLAG) BFS_ff.cpp
	
BFS_ff_mul: BFS_ff_mul.o Node.o graphfile_manager.o 
	$(CXX) $(CXXFLAGS) $(OFLAGS) -o BFS_ff BFS_ff_mul.o Node.o graphfile_manager.o

BFS_ff_mul.o: BFS_ff.cpp Node.o graphfile_manager.o
	$(CXX) $(CXXFLAGS) $(MUL) $(OFLAGS) $(IFLAG) BFS_ff.cpp -o BFS_ff_mul.o

Node.o: $(INC_DIR)
	$(CXX) $(CXXFLAGS) $(OFLAGS) -c $(INC_DIR)Node.cpp

graphfile_manager.o: $(INC_DIR)
	$(CXX) $(CXXFLAGS) $(OFLAGS) -c $(INC_DIR)graphfile_manager.cpp

clean: 
	rm *.o 

clean_exe: 
	rm BFS_ff 