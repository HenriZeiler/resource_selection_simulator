//
// Created by Henri Zeiler on 12.05.24.
//

#ifndef RESSOURCE_SELECTION_SIMULATOR_RESOURCE_H
#define RESSOURCE_SELECTION_SIMULATOR_RESOURCE_H

#include <limits.h>
#include <unordered_set>
#include "util.h"
#define INF INT_MAX
using namespace std;


typedef struct Resource {
    int max_slots = INF;
    unordered_set<int> actors_at_q;
    vector<double_t> fractions_of_types;

    Resource(const int max_slots, const int nr_of_types);
} Resource;


#endif //RESSOURCE_SELECTION_SIMULATOR_RESOURCE_H
