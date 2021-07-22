#include <iostream>
#include <vector>
#include <random>
#include <iterator>
#include <map>
#include <queue>
#include <fstream>
#include <string>
#include <cstring> 

#include "Node.h"
#include "graphfile_manager.h"
#include "utimer.cpp"

using namespace std;



void bfs(int id_starter, int value, map<int, Node*> &nodes, vector<long>& times){
#if TIMER
    long elapsed;
    int frontier_step = 0;
#endif
    long elapsed_tot;
    utimer t("");
    vector<Node*> frontier;
    vector<Node*> next_frontier;
    int count = 0;
    frontier = nodes.find(id_starter)->second->get_neighbours();
    for (Node* node : frontier) {
        node->discovered = true;
    }
    
    while (!frontier.empty()) {
        next_frontier.clear();
#if TIMER
        frontier_step++;
        utimer t("Next frontier", &elapsed);
#endif
        for (Node* node: frontier){
            if (node->visited) continue;
            node->visited = true;
            count += (int)(node->value == value);
            for (Node* nb: node->get_neighbours()){
                if(nb->discovered) continue;
                nb->discovered = true;
                next_frontier.push_back(nb);
            }
        }  
#if TIMER
        cout << "Frontier " << frontier_step << " size " << frontier.size() << endl;
        t.print();
#endif
        frontier.clear();
        swap(frontier, next_frontier);
    }
    //Stampa il numero di nodi con il valore richiesto
    cout << "Nodes with value are: " << count << "\n";
    elapsed_tot = t.getElapsedTime();
    times.push_back(elapsed_tot);
    cout << "BFS seq -> computed in " << elapsed_tot << " microseconds\n";
}


int main(int argc, char const *argv[]){

    if (argc < 3) cout<< "Input args-> id_starter value no_nodes\n";

    int id_starter = std::atoi(argv[1]);
    int value = std::atoi(argv[2]);
    string filename = argv[3];

    //Mappa dei vertici del grafo
    std::map<int, Node*> nodes;
    //Leggo il grafo da file
    cout << "LEGGO FILE\n";
    read_graph(filename, nodes);
    cout << "START FARM\n";
    vector<long> times;
#if MUL
    map<int, Node *>::iterator it;
    for (uint i = 0; i < 10; i++) {
      bfs(id_starter, value, nodes, times);
      for (it = nodes.begin(); it != nodes.end(); it++)
      {
        it->second->discovered = false;
        it->second->visited = false;
      }
    }
    long sum = 0;
    long min = times[0];
    for (uint i = 0; i < times.size(); i++){
        if(min > times[i]) min = times[i];
      long time = times[i];
      sum += time;
  }
    long avg = sum/times.size();
    cout << "AVG " << avg << endl;
    cout << "Min " << min << endl;
#else
    {
        bfs(id_starter, value, nodes, times);
    }
#endif
    
    

    return 0;
}
