CXX = g++
INC_DIR = ../graph/
CXXFLAGS = -std=c++11 
IFLAG = -c -Wall -I$(INC_DIR)
OFLAGS = -O3



graph_generator: graph_generator.o Node.o graphfile_manager.o 
	$(CXX) $(CXXFLAGS) $(OFLAGS) -o graph_generator graph_generator.o Node.o graphfile_manager.o

# The main.o target can be written more simply

graph_generator.o: graph_generator.cpp Node.o graphfile_manager.o
	$(CXX) $(CXXFLAGS) $(OFLAGS) $(IFLAG) graph_generator.cpp

graph_generator_er: graph_generator_er.o Node.o graphfile_manager.o 
	$(CXX) $(CXXFLAGS) $(OFLAGS) -o graph_generator_er graph_generator_er.o Node.o graphfile_manager.o

# The main.o target can be written more simply

graph_generator_er.o: graph_generator_er.cpp Node.o graphfile_manager.o
	$(CXX) $(CXXFLAGS) $(OFLAGS) $(IFLAG) graph_generator_er.cpp

Node.o: $(INC_DIR)
	$(CXX) $(CXXFLAGS) $(OFLAGS) -c $(INC_DIR)Node.cpp

graphfile_manager.o: $(INC_DIR)
	$(CXX) $(CXXFLAGS) $(OFLAGS) -c $(INC_DIR)graphfile_manager.cpp

clean: 
	rm *.o 

clean_exe: 
	rm graph_generator