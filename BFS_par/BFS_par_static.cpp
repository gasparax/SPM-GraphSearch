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
#include <algorithm>
#include <unordered_set>

#include "utimer.cpp"
#include "Node.h"
#include "graphfile_manager.h"
#include "acout.hpp"
#include "myqueue.cpp"


Node* STOPPER = new Node(-1, -1);
Node* EOS = new Node(-2, -2);
pair<int, int> STOP_EXE(-1, -1);
pair<int, int> EOT(-1, 0);
std::condition_variable cv;
mutex mtx;
#if TIMER
mutex mtx_print;
#endif
bool new_frontier = true;

using namespace std;

void source2farm(vector<Node*> &frontier, vector<myqueue<pair<int, int>>> &qin, int nw) {
#if TIMER
  long elapsed;
  long total_elapsed = 0;
  int frontier_step = 0;
  pair<int, long> max_time;
  vector<pair<int, long>> times;
  vector<int> frontier_sizes;
#endif
  while (true) {
    std::unique_lock<std::mutex> lk(mtx);
    cv.wait(lk, []{return new_frontier;});
    if (!frontier.empty()) {
#if TIMER
      utimer t("Emitter time->", &elapsed);
      frontier_step++;
      frontier_sizes.push_back(frontier.size());
#endif
      if (frontier.front() == STOPPER) {
        for (int i = 0; i < nw; i++) {
          qin[i].push(STOP_EXE);  
        }
#if TIMER
      {
        lock_guard<mutex> lg(mtx_print);
        acout() << "--------------------------------------------------------------------------------------\n";
        acout() << "Emitter " << endl;
        for (uint i = 0; i < times.size(); i++) {
          acout() << "Frontier " << times[i].first <<" size -> " << frontier_sizes[i] << endl;
          acout() << "Frontier " << times[i].first <<" time -> " << times[i].second << endl;
        }
        acout() << "Total emitter time -> " << total_elapsed << endl;
        acout() << "Max emitter time -> Frontier " << max_time.first << " " << max_time.second << endl;
        acout() << "AVG emitter time -> " << total_elapsed/frontier_step << endl;
        acout() << "--------------------------------------------------------------------------------------\n";
      }
#endif
        return;
      }
      int node_per_worker = ceil((double)frontier.size()/nw);        
      unsigned start_pos = 0;
      unsigned end_pos = node_per_worker;
      int i = 0;
      while (end_pos <= frontier.size()) {
        pair<int, int> w_pair(start_pos, end_pos);
        qin[i].push(w_pair);
        i++;
        start_pos = end_pos;       
        end_pos += node_per_worker;
        if (start_pos < frontier.size() && end_pos >= frontier.size()){end_pos = frontier.size();} 
      }
      for (int i = 0; i < nw; i++){
        qin[i].push(EOT);
      }
      new_frontier = false;
      lk.unlock();
      cv.notify_one();
#if TIMER
        elapsed = t.getElapsedTime();
        pair<int, long> frontier_time(frontier_step, elapsed);
        times.push_back(frontier_time);
        if (max_time.second < elapsed)
        {
          max_time.first = frontier_step;
          max_time.second = elapsed;
        }
        total_elapsed += elapsed;
#endif
    }
  }
}       

void genericfarmstage(vector<Node*> &frontier, 
    myqueue<pair<int, int>> &qin, 
    vector<Node*> &qout, 
    atomic<int> &counter, 
    atomic<int> &end_of_task, int value, int id){
#if TIMER
  long elapsed;
  long total_elapsed = 0;
  int chunk_step = 0;
  pair<int, long> max_time;
  vector<pair<int, long>> times;
  vector<pair<int, long>> wait_times;
  vector<int> frontier_sizes;
  vector<int> frontier_new_nodes;
  vector<int> frontier_not_nodes;
  int new_nodes = 0;
  int visited_nodes = 0;
  int not_visited_nodes = 0;
#endif
  int local_counter = 0;
  while (true){
#if TIMER 
    utimer wait_timer("Waiting for -> ");
#endif
    if (!qin.empty()){
#if TIMER

#endif      
      pair<int, int> input_pair = qin.pop();
      if (input_pair == STOP_EXE) {
        counter+=local_counter;
#if TIMER
      {
        lock_guard<mutex> lg(mtx_print);
        acout() << "--------------------------------------------------------------------------------------\n";
        acout() << "Worker " << id << endl;
        for (uint i = 0; i < times.size(); i++) {
          acout() << "Chunk Frontier " << times[i].first <<" size -> " << frontier_sizes[i]<< endl;
          acout() << "Chunk Frontier " << times[i].first <<" wait time -> " << wait_times[i].second << endl;
          acout() << "Chunk Frontier " << times[i].first <<" time -> " << times[i].second << endl;
          acout() << "Chunk Frontier " << times[i].first <<" new nodes -> " << frontier_new_nodes[i] << endl;
          acout() << endl;  
        }
        acout() << "Visisted nodes " << visited_nodes << endl;
        acout() << "Total time -> " << total_elapsed << endl;
        acout() << "Max time -> Chunk " << max_time.first << " Time " << max_time.second << endl;
        acout() << "AVG time -> " << total_elapsed/chunk_step << endl;
        acout() << "--------------------------------------------------------------------------------------\n";
      }
#endif
        return;
      }
      if (input_pair == EOT) {
        end_of_task.fetch_add(1);
      } else {
#if TIMER
        pair<int, long> p(chunk_step, wait_timer.getElapsedTime());
        wait_times.push_back(p);
        wait_timer.restart();
        frontier_sizes.push_back(input_pair.second-input_pair.first);
        utimer t("");
        chunk_step++;
#endif    
          for (int i = input_pair.first; i < input_pair.second; i++) {
            Node *node = frontier.at(i);
            bool expected = false;
            if(!node->visited.compare_exchange_weak(expected, true)){
              continue;
            } 
#if TIMER
            visited_nodes++;
#endif
            local_counter += (int)(node->value == value);
            for (Node *nb : node->get_neighbours()) {
              if (nb->discovered) {
#if TIMER
                not_visited_nodes++;
#endif
                continue;
              }
#if TIMER
              new_nodes++;
#endif
              nb->discovered = true;
              qout.push_back(nb);
            }
          }
#if TIMER
        frontier_new_nodes.push_back(new_nodes);
        new_nodes = 0;
        frontier_not_nodes.push_back(not_visited_nodes);
        not_visited_nodes = 0;
        elapsed = t.getElapsedTime();
        pair<int, long> chunk_time(chunk_step, elapsed);
        times.push_back(chunk_time);
        if (max_time.second < elapsed)
        {
          max_time.first = chunk_step;
          max_time.second = elapsed;
        }
        total_elapsed += elapsed;
#endif
      }
    }
  }
}

void drainfarm(vector<vector<Node *>> &qout, vector<Node *> &frontier, atomic<int> &end_of_task, int nw) {
#if TIMER
  long elapsed;
  long total_elapsed = 0;
  int next_frontier_step = 0;
  pair<int, long> max_time;
  vector<pair<int, long>> times;
#endif
  vector<Node *> next_frontier;
  while (true) {
      std::unique_lock<std::mutex> lk(mtx);
      cv.wait(lk, []
              { return !new_frontier; });
    if (end_of_task == nw) {
#if TIMER
      string m = "Collector time->";
      utimer t(m, &elapsed);
      next_frontier_step++;
#endif
      for (int i = 0; i < nw; i++) {
        if (!qout[i].empty()) {
          next_frontier.insert(next_frontier.end(), qout[i].begin(), qout[i].end());
          qout[i].clear();
        }
      }
      if (next_frontier.empty()) {
        frontier.clear();
        frontier.push_back(STOPPER);
#if TIMER
      {
        lock_guard<mutex> lg(mtx_print);
        acout() << "--------------------------------------------------------------------------------------\n";
        acout() << "Collector " << endl;
        for (uint i = 0; i < times.size(); i++) {
          acout() << "Frontier " << times[i].first <<" time -> " << times[i].second << endl;
        }
        acout() << "Total collector time -> " << total_elapsed << endl;
        acout() << "Max collector time -> Next F " << max_time.first << " Time " << max_time.second << endl;
        acout() << "AVG collector time -> " << total_elapsed / next_frontier_step << endl;
        acout() << "--------------------------------------------------------------------------------------\n";
      }
#endif
        new_frontier = true;
        cv.notify_one();
        return;
      }
      frontier.clear();
      frontier.swap(next_frontier);
      new_frontier = true;
      cv.notify_one();
      end_of_task = 0;
#if TIMER
        elapsed = t.getElapsedTime();
        pair<int, long> frontier_time(next_frontier_step, elapsed);
        times.push_back(frontier_time);
        if (max_time.second < elapsed)
        {
          max_time.first = next_frontier_step;
          max_time.second = elapsed;
        }
        total_elapsed += elapsed;
#endif
    }
  }
}

void farm(map<int, Node*> nodes, int id_starter, int value, int nw, vector<long>& times) {
  long elapsed;
  vector<Node*> frontier;
  vector<myqueue<pair<int, int>>> qin(nw);// input deques for the workers 
  vector<vector<Node*>> qout(nw);// input deques for the workers 
  atomic<int> end_of_task(0);
  atomic<int> counter(0);
  vector<atomic<bool>> checked(nodes.size());  
  // now start the farm stages threads
  for (Node* n : frontier = nodes.find(id_starter)->second->get_neighbours()){
    n->discovered = true;
  }
  utimer t("BFS PAR", &elapsed);  
  counter += (int)(nodes.find(id_starter)->second->id == value);
  nodes.find(id_starter)->second->discovered = true;
  frontier = nodes.find(id_starter)->second->get_neighbours();
  std::thread s0(source2farm, 
    std::ref(frontier), 
    std::ref(qin),
    nw);
  std::vector<std::thread*> tids(nw);
  for(int i=0; i<nw; i++) {
    tids[i] = new std::thread(genericfarmstage, 
      std::ref(frontier),
      std::ref(qin[i]), 
      std::ref(qout[i]), 
      std::ref(counter),
      std::ref(end_of_task),
      value,
      i);  
  } 
  std::thread s1(drainfarm, 
    std::ref(qout),
    std::ref(frontier),
    std::ref(end_of_task),
    nw);
                                
  s0.join(); 
  for(int i=0; i<nw; i++) 
      tids[i]->join();
  s1.join();
#if MUL
  times.push_back(t.getElapsedTime());
#else
cout << "VALORI TROVATI\n" << counter << endl;
  t.print();
  times.push_back(t.getElapsedTime());
#endif
}

void save_results(vector<long> times, int no_nodes){
    // Create and open a text file
    string filename = "results/results_BFS_map_";
    filename.append(to_string(no_nodes)).append(".txt");
    ofstream resultsFile(filename);
    for (unsigned i = 0; i < times.size(); i++){
        //cout << node->id << endl;
        //Write on the file
        string file_row = to_string(i+1);
        file_row.append(" ").append(to_string(times.at(i))).append("\n");
        resultsFile << file_row;
    }

  // Close the file
  resultsFile.close();
}

int main(int argc, char const *argv[]){

  if (argc < 4) cout << "Input args-> id_starter value max_worker filename\n";

  int id_starter = std::atoi(argv[1]);
  int value = std::atoi(argv[2]);
  int max_worker = std::atoi(argv[3]);
  string filename = argv[4];

  //Mappa dei vertici del grafo
  std::map<int, Node*> nodes;

  //Leggo il grafo da file
  cout << "LEGGO FILE\n";
  read_graph(filename, nodes);
  cout << "START FARM\n";

#if MUL
  vector<long> avg_times;
  for (int i = 1; i <= max_worker; i++) {
    map<int, Node *>::iterator it;
    vector<long> times;
    for (uint j = 0; j < 10; j++) {
      farm(nodes, id_starter, value, i, times);
      //cout << "Time " << times[j] << endl;
      for (it = nodes.begin(); it != nodes.end(); it++)
      {
        it->second->discovered = false;
        it->second->visited = false;
      }
    }
    long sum = 0;
    for (uint z = 0; z < times.size(); z++){
      
      long time = times[z];
      sum += time;
    }
    long avg = sum/times.size();
    avg_times.push_back(avg);
    cout << i << " " << avg << endl;
  }
  save_results(avg_times, nodes.size());
#else
  vector<long> times;
  farm(nodes, id_starter, value, max_worker, times);
#endif 
  return 0;
}