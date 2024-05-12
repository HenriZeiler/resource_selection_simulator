//
// Created by Henri Zeiler on 12.05.24.
//

#ifndef RESSOURCE_SELECTION_SIMULATOR_ACTOR_H
#define RESSOURCE_SELECTION_SIMULATOR_ACTOR_H

#include <vector>
#include <functional>
#include "Simulator.h"
#include "Resource.h"

using namespace std;
typedef int actor_type;
typedef double function_space;   //should be Simulator::function_space

class Actor {
    const int idx;
    const actor_type type;
    int cur_resource;
    const vector<int> available_resources;
    int get_cur_resource();
    actor_type get_type();
    function_space get_util(function<function_space(function_space)> util_function);

    bool switch_to_best_resource();         //returns true if a switch took place
    bool cur_resource_still_available();
public:
    Actor(const int idx, const actor_type type,vector<int> available_resources);
};


#endif //RESSOURCE_SELECTION_SIMULATOR_ACTOR_H
