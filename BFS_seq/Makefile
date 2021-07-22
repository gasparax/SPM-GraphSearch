CXX = g++
INC_DIR = ../graph/
CXXFLAGS = -std=c++11 -pthread
IFLAG = -c -Wall -I$(INC_DIR)
OFLAGS = -O3
TIMER = -DTIMER
MUL_EXE = -DMUL



BFS_seq: BFS_seq.o Node.o graphfile_manager.o 
	$(CXX) $(CXXFLAGS) $(OFLAGS) -o BFS_seq BFS_seq.o Node.o graphfile_manager.o

BFS_seq.o: BFS_seq.cpp Node.o graphfile_manager.o
	$(CXX) $(CXXFLAGS) $(OFLAGS) $(IFLAG) BFS_seq.cpp

BFS_seq_timer: BFS_seq_timer.o Node.o graphfile_manager.o 
	$(CXX) $(CXXFLAGS) $(OFLAGS) -o BFS_seq BFS_seq_timer.o Node.o graphfile_manager.o

BFS_seq_timer.o: BFS_seq.cpp Node.o graphfile_manager.o
	$(CXX) $(CXXFLAGS) $(TIMER) $(OFLAGS) $(IFLAG) BFS_seq.cpp -o BFS_seq_timer.o

BFS_seq_mul: BFS_seq_mul.o Node.o graphfile_manager.o 
	$(CXX) $(CXXFLAGS) $(OFLAGS) -o BFS_seq BFS_seq_mul.o Node.o graphfile_manager.o

BFS_seq_mul.o: BFS_seq.cpp Node.o graphfile_manager.o
	$(CXX) $(CXXFLAGS) $(MUL_EXE) $(OFLAGS) $(IFLAG) BFS_seq.cpp -o BFS_seq_mul.o
	
Node.o: $(INC_DIR)
	$(CXX) $(CXXFLAGS) $(OFLAGS) -c $(INC_DIR)Node.cpp

graphfile_manager.o: $(INC_DIR)
	$(CXX) $(CXXFLAGS) $(OFLAGS) -c $(INC_DIR)graphfile_manager.cpp

clean: 
	rm *.o 

clean_exe: 
	rm BFS_seq