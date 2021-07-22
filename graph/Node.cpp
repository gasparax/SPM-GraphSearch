#include "Node.h"

Node::Node(int input_id, int value_in){
    id = input_id;
    value = value_in;
    discovered = false;
    visited = false;
}

void Node::add_neighbour(Node* neighbour){
    neighbours.push_back(neighbour);
}

void Node::sort_neighbours(){
    sort (neighbours.begin(), neighbours.end(), 
        [](Node* a, Node* b) -> bool
        { return a->id < b->id; } );
}

vector<Node*> Node::get_neighbours(){
    return neighbours;
}

void Node::print_neighbours(){
    std::cout << "Neighbours of node " << id << endl;
    if (neighbours.size()==0){
        std::cout << "No neighbours\n";
        return;
    }
    std::cout << "{";
    for (Node* n : neighbours){
        std::cout << " "<< n->id;
    }
    cout << " }\n";
}

bool Node::neighbourPresent(int id){
    for (Node* n : neighbours){
        if (n->id == id){
            return true;
        }
    }
    return false;           
}