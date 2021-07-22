#include "graphfile_manager.h"

void tokenize(std::string const &str, const char* delim, std::vector<std::string> &out) {
    char *token = strtok(const_cast<char *>(str.c_str()), delim);
    while (token != nullptr)
    {
        out.push_back(std::string(token));
        token = strtok(nullptr, delim);
    }
}

void read_graph(string file, map<int, Node *> &nodes)
{
    // Create a text string, which is used to output the text file
    string node_row;
    string filename = "../graph/graph_files/";
    filename.append(file);
    cout << filename << endl;
    // Read from the text file
    ifstream GraphFile(filename);

    // Use a while loop together with the getline() function to read the file line by line
    //Aggiungo i nodi alla map
    while (getline(GraphFile, node_row))
    {
        // Output the text from the file
        std::vector<string> node_details;
        tokenize(node_row, " ", node_details);
        int node_id = stoi(node_details[0]);
        int node_value = stoi(node_details[1]);
        Node *new_node = new Node(node_id, node_value);
        nodes.insert(pair<int, Node *>(node_id, new_node));
        //cout << "Added node " << nodes.find(node_id)->second->id << endl;
        node_details.clear();
    }

    GraphFile.clear();
    GraphFile.seekg(0);

    //Leggo gli archi da file
    map<int, Node *>::iterator itr;

    while (getline(GraphFile, node_row))
    {
        // Output the text from the file
        //acout() << node_row << endl;
        std::vector<string> node_details;
        tokenize(node_row, " ", node_details);
        int node_id = stoi(node_details[0]);
        for (int i = 2; i < node_details.size(); i++)
        {
            nodes.find(node_id)->second->add_neighbour(nodes.find(stoi(node_details[i]))->second);
        }
        node_details.clear();
    }

    // Close the file
    GraphFile.close();
}
