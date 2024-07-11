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
    const bool empty_neighbourhood_optimal = false;
    const bool impact_aware = false;
    const vector<vector<bool>> allowed_swap_configs;        // {{before->before,before->behind},{behind->before,behind->behind}}
    const function_space min_improvement = 0;
    vector<Actor> actors;
    vector<Resource> resources;
    function<function_space(function_space)> utility_function;
    const function_space peak;

    bool resource_q_available(const Resource q);
    bool swap_resource_for_actor(Actor& a);
    function_space get_cur_util_for_actor(const Actor& a, const bool measure_segregation=false);
    function_space get_total_utility(const function<function_space(function_space)> utility_function);
    function_space get_total_segregation_welfare(const function<function_space(function_space)> utility_function);
    int step();     //returns number of swaps in step

public:
    function_space get_util_for_type_at_q(const actor_type t, const Resource q, const function<function_space(function_space)> custom_util, const bool measure_segregation = false);
    double_t get_fraction_for_type_at_q(actor_type t, Resource q);
    int run_simulation(int steps, int data_collection_interval, vector<function_space>& social_welfare_at_collection_step, vector<vector<vector<function_space>>>& fraction_at_q_at_collection_step, vector<function_space>& segregation_welfare_at_collection_step);
    Simulator(const vector<Actor> actors, const vector<Resource> resources, const function<function_space(function_space)> utility_function, const function_space peak, const function_space epsilon = 0, const bool isolation_optimal = false, const bool impact_aware = false, const vector<vector<bool>> allowed_swap_configs = {{true,true},
                                                                                                                                                                                                                                                                                                                          {true,true}});
} Simulator;


#endif //RESSOURCE_SELECTION_SIMULATOR_SIMULATOR_H
