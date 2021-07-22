#ifndef GRAPHFILE_MANAGER_H
#define GRAPHFILE_MANAGER_H
#include <map>
#include <string>
#include <fstream>
#include <cstring>
#include "Node.h"

void read_graph(string file, map<int, Node*> &nodes);

#endif