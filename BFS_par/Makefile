CXX = g++
INC_DIR = ../graph/
CXXFLAGS = -std=c++17 -pthread -g
IFLAG = -c -Wall -I$(INC_DIR)
OFLAGS = -O3
TIMER = -DTIMER
MUL = -DMUL


BFS_par_static: BFS_par_static.o Node.o graphfile_manager.o 
	$(CXX) $(CXXFLAGS) $(OFLAGS) -o BFS_par_static BFS_par_static.o Node.o graphfile_manager.o

BFS_par_static.o: BFS_par_static.cpp Node.o graphfile_manager.o
	$(CXX) $(CXXFLAGS) $(OFLAGS) $(IFLAG) BFS_par_static.cpp 

BFS_par_static_timer: BFS_par_static_timer.o Node.o graphfile_manager.o 
	$(CXX) $(CXXFLAGS) $(OFLAGS) -o BFS_par_static BFS_par_static_timer.o Node.o graphfile_manager.o

BFS_par_static_timer.o: BFS_par_static.cpp Node.o graphfile_manager.o
	$(CXX) $(CXXFLAGS) $(OFLAGS) $(TIMER) $(IFLAG) BFS_par_static.cpp -o BFS_par_static_timer.o

BFS_par_static_mul: BFS_par_static_mul.o Node.o graphfile_manager.o 
	$(CXX) $(CXXFLAGS) $(OFLAGS) -o BFS_par_static BFS_par_static_mul.o Node.o graphfile_manager.o

BFS_par_static_mul.o: BFS_par_static.cpp Node.o graphfile_manager.o
	$(CXX) $(CXXFLAGS) $(OFLAGS) $(MUL) $(IFLAG) BFS_par_static.cpp -o BFS_par_static_mul.o

BFS_par_dy: BFS_par_dy.o Node.o graphfile_manager.o 
	$(CXX) $(CXXFLAGS) $(OFLAGS) -o BFS_par_dy BFS_par_dy.o Node.o graphfile_manager.o

BFS_par_dy.o: BFS_par_dy.cpp Node.o graphfile_manager.o
	$(CXX) $(CXXFLAGS) $(OFLAGS) $(IFLAG) BFS_par_dy.cpp

BFS_par_dy_timer: BFS_par_dy_timer.o Node.o graphfile_manager.o 
	$(CXX) $(CXXFLAGS) $(OFLAGS) -o BFS_par_dy BFS_par_dy_timer.o Node.o graphfile_manager.o

BFS_par_dy_timer.o: BFS_par_dy.cpp Node.o graphfile_manager.o
	$(CXX) $(CXXFLAGS) $(OFLAGS) $(TIMER) $(IFLAG)  BFS_par_dy.cpp -o BFS_par_dy_timer.o

BFS_par_dy_mul: BFS_par_dy_mul.o Node.o graphfile_manager.o 
	$(CXX) $(CXXFLAGS) $(OFLAGS) -o BFS_par_dy BFS_par_dy_mul.o Node.o graphfile_manager.o

BFS_par_dy_mul.o: BFS_par_dy.cpp Node.o graphfile_manager.o
	$(CXX) $(CXXFLAGS) $(OFLAGS) $(MUL) $(IFLAG) BFS_par_dy.cpp -o BFS_par_dy_mul.o

Node.o: $(INC_DIR)
	$(CXX) $(CXXFLAGS) $(OFLAGS) -c $(INC_DIR)Node.cpp

graphfile_manager.o: $(INC_DIR)
	$(CXX) $(CXXFLAGS) $(OFLAGS) -c $(INC_DIR)graphfile_manager.cpp

clean: 
	rm *.o 

clean_exe: 
	rm BFS_par_static BFS_par_dy
