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
using namespace std;

void add_nodes(int n, map<int, Node*> &nodes){
    // Aggiungo i vertici al grafo con id incremetale
    for (int i = 0; i < n; i++){
        int value = rand()%(51);
        //i sarà l'id
        Node* node = new Node(i, value);
        nodes.insert(pair<int, Node*>(i, node)); 
    }
}

void add_edges(int n, map<int, Node*> &nodes, float density){
    // Aggiungo gli archi ai vertici
    map<int, Node*>::iterator itr;
    for (itr = nodes.begin(); itr != nodes.end(); itr++){
        Node* node = itr->second;
        for (int i = node->id+1; i < n; i++) {
            Node* new_nb = nodes.find(i)->second;
            float new_edge = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
            if (density >= new_edge) {
                node->add_neighbour(new_nb);
            }
        }
    }
}

void save_graph(map<int, Node*> nodes, float density){
    // Create and open a text file
    cout << "Starting save graph on disk\n";
    string filename = "graph_files/graph";
    filename.append(to_string(nodes.size())).append("").append(to_string(density)).append(".txt");
    ofstream GraphFile(filename);
    map<int, Node*>::iterator itr;
    for (itr = nodes.begin(); itr != nodes.end(); itr++){
        Node* node = itr->second;
        //cout << node->id << endl;
        //Write on the file
        string file_row = to_string(node->id);
        file_row.append(" ").append(to_string(node->value));
        for (Node* n :node->get_neighbours()){
            file_row.append(" ").append(to_string(n->id));
        }
        file_row.append("\n");
        GraphFile << file_row;
        file_row = "";
    }

  // Close the file
  GraphFile.close();

}

int main(int argc, char const *argv[]){

    if (argc < 2) cout<< "Input args-> vertex max_link\n";
    int n = std::atoi(argv[1]);
    float density = std::atof(argv[2]);

    srand(123);

    //Mappa dei vertici del grafo
    std::map<int, Node*> nodes;
    add_nodes(n,nodes);
    cout << "Genero archi\n";
    add_edges(n,nodes,density);
    map<int, Node*>::iterator itr;

    save_graph(nodes, density);

    return 0;
}
