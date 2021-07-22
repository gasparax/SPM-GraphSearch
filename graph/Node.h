#ifndef NODE_H
#define NODE_H
#include <iostream>
#include <vector>
#include <algorithm>
#include <atomic>

using namespace std;

class Node 
{
public:
    int id;
    int value;
    atomic<bool> discovered;
    atomic<bool> visited;
    vector<Node*> neighbours;

    Node(int input_id, int value_in);

    void add_neighbour(Node* neighbour);

    void sort_neighbours();

    vector<Node*> get_neighbours();

    void print_neighbours();

    bool neighbourPresent(int id);

};

#endif