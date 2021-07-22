#include <iostream>
#include <mutex>
#include <condition_variable>
#include <deque>
#include <vector>
#include <deque>
#include <map>
#include <cstddef>
#include <math.h>
#include <atomic> 
#include <condition_variable>
#include <thread>
#include <math.h> 

#include "utimer.cpp"
#include "Node.h"
#include "graphfile_manager.h"
#include "acout.hpp"

#include <iostream>
#include <ff/ff.hpp>
#include <ff/parallel_for.hpp>
using namespace ff;

vector<atomic<bool>> counted;

struct Task {
    vector<Node*> next_frontier;
    pair<uint, uint> chunk_size;
    int counter = 0;
    Task(pair<uint, uint> chunk_size)
        : chunk_size(chunk_size) {}
};


struct Emitter: ff_monode_t<Task> {
    const int nw;
    int counter = 0;
    int frontier_count = 0;
    vector<Node*> &frontier;
    vector<Node*> next_frontier;
    int emitted_task = 0;
    Emitter(uint nw, vector<Node*>& frontier) : nw(nw), frontier(frontier){}
    
    Task* svc(Task* in){
        if (in == nullptr) {
            int chunk_size = ceil((double)frontier.size()/nw);
            uint start_pos = 0;
            uint end_pos = start_pos + chunk_size;
            int i = 0;
            while (end_pos <= frontier.size()) {
                pair<uint, uint> chunk(start_pos, end_pos);
                Task* task = new Task(chunk);
                ff_send_out(task);
                emitted_task++;
                i++;
                start_pos = end_pos;
                end_pos += chunk_size;
                if (start_pos < frontier.size() && end_pos >= frontier.size()){
                    end_pos = frontier.size();
                } 
            }
            return GO_ON;
        }
        auto V = in->next_frontier;
        if (V.size()) {
            next_frontier.insert(next_frontier.end(), V.begin(), V.end());  
        }
        counter+=in->counter;
        emitted_task--;
        delete in;
        if (emitted_task == 0){
            if (next_frontier.empty()) {
#if MUL
#else
                acout() << "Found -> " << counter << endl;
#endif
                broadcast_task(EOS);
                return EOS;
            }
            frontier.clear();
            frontier.swap(next_frontier);
            frontier_count++;
            int chunk_size = ceil((double)frontier.size()/nw);
            uint start_pos = 0;
            uint end_pos = start_pos + chunk_size;
            while (end_pos <= frontier.size()) {
                pair<int, int> chunk(start_pos, end_pos);
                Task* task = new Task(chunk);
                ff_send_out(task);
                emitted_task++;
                start_pos = end_pos;
                end_pos += chunk_size;
                if (start_pos < frontier.size() && end_pos >= frontier.size()){
                    end_pos = frontier.size();
                }   
            }
            return GO_ON;
        } else {
            return GO_ON;
        }
    }
};

struct Worker: ff_node_t<Task> {

    int value;
    vector<Node*> &frontier;
    Worker(int value, vector<Node*>& frontier) : value(value), frontier(frontier) {}
    
    Task* svc(Task * task) {
        int local_counter = 0;
        if(!frontier.empty()) {
            vector<Node*> next_frontier;
            unsigned start_pos = task->chunk_size.first;
            unsigned end_pos = task->chunk_size.second;
            for (uint i = start_pos; i < end_pos; i++) {
                Node* node = frontier.at(i);
                bool exp = false;
                if(!node->visited.compare_exchange_strong(exp, true)) continue;
                local_counter += (int)(node->value == value);
                for (Node* nb : node->get_neighbours()) {
                    if (nb->discovered) continue;
                    nb->discovered = true;
                    next_frontier.push_back(nb);
                }
            }
            task->next_frontier.swap(next_frontier);
            task->counter = local_counter;
            ff_send_out(task);
            return GO_ON;
        }
        return GO_ON;
    }
};

int main(int argc, char const *argv[]){

    if (argc < 4)
        cout << "Input args-> id_starter value max_wokers filename\n";

    int id_starter = std::atoi(argv[1]);
    int value = std::atoi(argv[2]);
    uint max_wokers = std::atoi(argv[3]);
    string filename = argv[4];

    //Mappa dei vertici del grafo
    std::map<int, Node*> nodes;

    //Leggo il grafo da file
    cout << "LEGGO FILE\n";
    read_graph(filename, nodes);
    cout << "START FARM\n";

#if MUL
    vector<long> avg_times;
    for (uint i = 1; i <= max_wokers; i++) {
        vector<long> times_run;
        for (uint j = 0; j < 9; j++) {
            ff::ffTime(START_TIME);
            vector<Node *> frontier;
            Node *starter = nodes.find(id_starter)->second;
            frontier = starter->get_neighbours();
            ff_Farm<> farm([&]()
                        {
                            std::vector<std::unique_ptr<ff_node>> W;
                            for (size_t y = 0; y < i; ++y)
                                W.push_back(make_unique<Worker>(value, frontier));
                            return W;
                        }());

            Emitter E(i, frontier);
            farm.add_emitter(E);    
            farm.remove_collector();
            farm.wrap_around();     

            if (farm.run_and_wait_end() < 0) {
                error("running farm\n");
                return -1;
            }
            ff::ffTime(STOP_TIME);
            long time = farm.ffTime() * 1000;
            times_run.push_back(time);
            map<int, Node *>::iterator it;
            for (it = nodes.begin(); it != nodes.end(); it++) {
                it->second->discovered = false;
                it->second->visited = false;
            }
        }
        long sum = 0;
        for (uint z = 0; z < times_run.size(); z++) {
            long time = times_run[z];
            cout << time << endl;
            sum += time;
        }
        long avg = sum / times_run.size();
        avg_times.push_back(avg);
        cout << i << " " << avg << endl;
        times_run.clear();
    }
#else
    ff::ffTime(START_TIME);
    vector<Node *> frontier;
    Node *starter = nodes.find(id_starter)->second;
    frontier = starter->get_neighbours();
    ff_Farm<> farm([&]()
                   {
                       std::vector<std::unique_ptr<ff_node>> W;
                       for (size_t y = 0; y < max_wokers; ++y)
                           W.push_back(make_unique<Worker>(value, frontier));
                       return W;
                   }()); 

    Emitter E(max_wokers, frontier);  
    farm.add_emitter(E);     
    farm.remove_collector(); 
    farm.wrap_around();      

    if (farm.run_and_wait_end() < 0)
    {
        error("running farm\n");
        return -1;
    }
    ff::ffTime(STOP_TIME);
    std::cout << "BFS_FF: " << farm.ffTime() * 1000 << " (ms)\n";
#endif

  return 0;
}