//
// Created by Henri Zeiler on 12.05.24.
//

#include "Simulator.h"

bool Simulator::resource_q_available(int q_idx) {
    return resources[q_idx].max_slots > resources[q_idx].actors_at_q.size();
}

bool Simulator::swap_resource_for_actor(int a_idx) {
    function_space max_util = -1;
    int max_q_idx = -1;
    for(auto q_idx:actors[a_idx].available_resources) {
        if(resource_q_available(q_idx) && utility_function(resources[q_idx].fractions_of_types[actors[a_idx].type])>max_util) {
                max_util = utility_function(resources[q_idx].fractions_of_types[actors[a_idx].type]);
                max_q_idx = q_idx;
            }
        }
    if(max_q_idx != actors[a_idx].cur_resource) {
        actors[a_idx].cur_resource = max_q_idx;
        return 1;
    }
    return 0;
}

bool Simulator::check_for_nash_equilibrium() {
    return is_nash_stable;
}

function_space Simulator::get_total_utility(function<function_space(function_space)> utility_function) {
    function_space res = 0;
    for(auto a: actors) {
        res += utility_function(resources[a.cur_resource].fractions_of_types[a.type]);
    }
    return res;
}

Simulator::Simulator(const int number_of_types, vector<Actor> actors, vector<Resource> resources,
                     function<function_space(function_space)> utility_function) :
                     number_of_types(number_of_types), actors(actors), resources(resources), utility_function(utility_function) {
}