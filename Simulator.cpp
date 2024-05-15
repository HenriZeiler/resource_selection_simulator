//
// Created by Henri Zeiler on 12.05.24.
//

#include "Simulator.h"
#include <iostream>

bool Simulator::resource_q_available(const Resource q) {
    return q.max_slots > q.total_actors_at_q;
}

function_space Simulator::get_util_for_type_at_q(actor_type t, Resource q, function<function_space(function_space)> custom_util, bool measure_segregation) {
    if(q.total_actors_at_q) {
        if(measure_segregation) return custom_util((q.total_actors_at_q-q.actors_of_type[t])/q.total_actors_at_q);
        else return custom_util(q.actors_of_type[t]/q.total_actors_at_q);
    }
    else return 0;
}

bool Simulator::swap_resource_for_actor(Actor& a) {
    function_space max_util = (a.cur_resource != -1)? get_util_for_type_at_q(a.type,resources[a.cur_resource],utility_function): -1;
    int max_q_idx = a.cur_resource;
    for(auto q_idx:a.available_resources) {
        if(resource_q_available(resources[q_idx]) && get_util_for_type_at_q(a.type,resources[q_idx],utility_function) > max_util) {
            clog << "got util: " << get_util_for_type_at_q(a.type,resources[q_idx],utility_function) << " from " << q_idx << endl;
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
        clog << "actor type " << a.type << " swapped resource to " << a.cur_resource <<  endl;
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
    for(Actor a: actors) {
        res += get_util_for_type_at_q(a.type, resources[a.cur_resource],utility_function,true);
    }
    return res;
}

int Simulator::step() {
    int swaps_made = 0;
    for(auto& a:actors) {
        swaps_made += swap_resource_for_actor(a);
        clog << "after return: " << a.cur_resource << endl;
    }
    clog << "swaps made in step: " << swaps_made << endl;
    return swaps_made;
}

//returns step at which nash equil was reached or -1
int Simulator::run_simulation(int steps, int data_collection_interval, vector<function_space>& social_welfare_at_collection_step, vector<vector<vector<function_space>>>& fraction_at_q_at_collection_step_for_type, vector<function_space>& segregation_welfare_at_collection_step) {
    fraction_at_q_at_collection_step_for_type.resize(resources.size());
    for(int i=0;i<steps;i++) {
        if(!step()) {
            clog<< "nash equilibrium reached in step: " << i << endl;
            return i+1;
        }
        if(i%data_collection_interval==0) {
            social_welfare_at_collection_step.push_back(get_total_utility(utility_function));
            segregation_welfare_at_collection_step.push_back(get_total_segregation_welfare([](int x){return x;}));
            for(int q_idx=0;q_idx<resources.size();q_idx++) {
                fraction_at_q_at_collection_step_for_type[q_idx].resize(resources[0].nr_of_types);
                for(int t=0; t<resources[q_idx].nr_of_types; t++) {
                    if (resources[q_idx].total_actors_at_q) fraction_at_q_at_collection_step_for_type[q_idx][i].push_back(resources[q_idx].actors_of_type[t]/resources[q_idx].total_actors_at_q);
                    else fraction_at_q_at_collection_step_for_type[q_idx][i].push_back(0);
                }
            }
        }
    }
    return -1;
}

Simulator::Simulator(vector<Actor> actors, vector<Resource> resources,
                     function<function_space(function_space)> utility_function) :
                     actors(actors), resources(resources), utility_function(utility_function) {
}