//
// Created by Henri Zeiler on 12.05.24.
//
#ifndef RESSOURCE_SELECTION_SIMULATOR_ACTOR_H
#define RESSOURCE_SELECTION_SIMULATOR_ACTOR_H

#include <vector>
#include <functional>
#include "util.h"

using namespace std;

typedef struct Actor {
    //const int idx;
    const actor_type type;
    int cur_resource;   //does not keep track of cur util as that can change due to external behaviour
    const vector<int> available_resources;  //0-indexed for compatibility with simulator.actors
    Actor(const actor_type type,vector<int> available_resources);
} Actor;


#endif //RESSOURCE_SELECTION_SIMULATOR_ACTOR_H
