//
// Created by Henri Zeiler on 12.05.24.
//

#ifndef RESSOURCE_SELECTION_SIMULATOR_RESOURCE_H
#define RESSOURCE_SELECTION_SIMULATOR_RESOURCE_H

#include <limits.h>
#include <unordered_set>
#include "Simulator.h"
#define INF INT_MAX
using namespace std;


class Resource {
    const int idx;
    int max_slots = INF;
    unordered_set<int> actors_at_q;
    vector<double_t> fractions_of_types;

    unordered_set<int> get_actors_at_q();
    int get_max_slots();
    void set_max_slots(int v);
    void leave_resource(int a);
    void join_resource(int a);
public:
    Resource(int idx, const int max_slots, const int nr_of_types);
};


#endif //RESSOURCE_SELECTION_SIMULATOR_RESOURCE_H
