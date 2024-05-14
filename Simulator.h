//
// Created by Henri Zeiler on 12.05.24.
//
#ifndef RESSOURCE_SELECTION_SIMULATOR_SIMULATOR_H
#define RESSOURCE_SELECTION_SIMULATOR_SIMULATOR_H



#include <cstdlib>
#include <vector>
#include <functional>
#include "Resource.h"
#include "Actor.h"
#include "util.h"

using namespace std;

typedef class Simulator {
private:
    vector<Actor> actors;
    vector<Resource> resources;
    function<function_space(function_space)> utility_function;

    bool resource_q_available(Resource q);
    bool swap_resource_for_actor(Actor a);
    bool check_for_nash_equilibrium();
    function_space get_total_utility(function<function_space(function_space)> utility_function);
    int step();     //returns number of swaps in step
    int run_simulation(int steps, int data_collection_interval, vector<function_space>& social_welfare_at_collection_step, vector<vector<function_space>>& fraction_at_q_at_collection_step, vector<function_space>& segregation_welfare_at_collection_step);

public:
    const int number_of_types;
    function_space get_util_for_type_at_q(actor_type t, Resource q, function<function_space(function_space)> custom_util);
    Simulator(const int number_of_types, vector<Actor> actors, vector<Resource> resources, function<function_space(function_space)> utility_function);
} Simulator;


#endif //RESSOURCE_SELECTION_SIMULATOR_SIMULATOR_H
