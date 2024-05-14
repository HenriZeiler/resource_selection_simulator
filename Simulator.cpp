//
// Created by Henri Zeiler on 12.05.24.
//

#include "Simulator.h"
#include <iostream>

bool Simulator::resource_q_available(Resource q) {
    return q.max_slots > q.total_actors_at_q;
}

function_space Simulator::get_util_for_type_at_q(actor_type t, Resource q, function<function_space(function_space)> custom_util, bool measure_segregation) {
    if(q.total_actors_at_q) {
        if(measure_segregation) return custom_util((q.total_actors_at_q-q.actors_of_type[t])/q.total_actors_at_q);
        else return custom_util(q.actors_of_type[t]/q.total_actors_at_q);
    }
    else return 0;
}

bool Simulator::swap_resource_for_actor(Actor a) {
    function_space max_util = -1;
    int max_q_idx = -1;
    for(auto q_idx:a.available_resources) {
        if(resource_q_available(resources[q_idx]) && get_util_for_type_at_q(a.type,resources[q_idx],utility_function) > max_util) {
                max_util = get_util_for_type_at_q(a.type,resources[q_idx],utility_function);
                max_q_idx = q_idx;
            }
        }
    if(max_q_idx != a.cur_resource) {       //also handles case in which no adjacent resource has slots left and updates resources fractions
        if(a.cur_resource != -1) {
            resources[a.cur_resource].actors_of_type[a.type]--;
            resources[a.cur_resource].total_actors_at_q--;
        }
        a.cur_resource = max_q_idx;
        if(max_q_idx != -1) {
            resources[max_q_idx].actors_of_type[a.type]++;
            resources[max_q_idx].total_actors_at_q++;
        }
        return 1;
    }
    return 0;
}

function_space Simulator::get_total_utility(function<function_space(function_space)> utility_function) {
    function_space res = 0;
    for(auto a: actors) {
        res += get_util_for_type_at_q(a.type, resources[a.cur_resource],utility_function);
    }
    return res;
}

function_space Simulator::get_total_segregation_welfare(function<function_space(function_space)> utility_function) {
    function_space res = 0;
    for(auto a: actors) {
        res += get_util_for_type_at_q(a.type, resources[a.cur_resource],utility_function,true);
    }
    return res;
}

int Simulator::step() {
    int swaps_made = 0;
    for(auto a:actors) {
        swaps_made += swap_resource_for_actor(a);
    }
    return swaps_made;
}

//returns step at which nash equil was reached or -1
int Simulator::run_simulation(int steps, int data_collection_interval, vector<function_space>& social_welfare_at_collection_step, vector<vector<vector<function_space>>>& fraction_at_q_for_type_at_collection_step, vector<function_space>& segregation_welfare_at_collection_step) {
    for(int i=0;i<steps;i++) {
        if(i%data_collection_interval==0) {
            social_welfare_at_collection_step[i/data_collection_interval] = get_total_utility(utility_function);
            segregation_welfare_at_collection_step[i/ data_collection_interval] = get_total_segregation_welfare([](int x){return x;});
            for(int q_idx=0;q_idx=resources.size();q_idx++) {
                for(int t=0;t<resources[0].nr_of_types;t++) fraction_at_q_for_type_at_collection_step[q_idx].push_back(resources[q_idx].actors_of_type);
            }
        }
        if(!step()) {
            clog<< "nash equilibrium reached in step: " << i+1 << endl;
            return i+1;
        }
    }
    return -1;
}

Simulator::Simulator(vector<Actor> actors, vector<Resource> resources,
                     function<function_space(function_space)> utility_function) :
                     actors(actors), resources(resources), utility_function(utility_function) {
}