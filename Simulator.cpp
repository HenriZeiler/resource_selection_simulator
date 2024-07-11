//
// Created by Henri Zeiler on 12.05.24.
//

#include "Simulator.h"
#include <iostream>

bool Simulator::resource_q_available(const Resource q) {
    return q.max_slots > q.total_actors_at_q;
}

function_space Simulator::get_util_for_type_at_q(actor_type t, Resource q, function<function_space(function_space)> custom_util, const bool measure_segregation) {
    if(impact_aware) {
        if(q.total_actors_at_q) {
            if(measure_segregation) return (q.total_actors_at_q-q.actors_of_type[t])/(q.total_actors_at_q+1);
            else return custom_util((q.actors_of_type[t]+1)/(q.total_actors_at_q+1));
        }
        else return empty_neighbourhood_optimal*utility_function(peak);
    }
    else {
        if(q.total_actors_at_q) {
            if(q.total_actors_at_q == 1 && q.actors_of_type[t] == 1) return empty_neighbourhood_optimal*utility_function(peak);
            if(measure_segregation) return (q.total_actors_at_q-q.actors_of_type[t])/q.total_actors_at_q;
            else return custom_util(q.actors_of_type[t]/q.total_actors_at_q);
        }
        else return 0;
    }
}

double_t Simulator::get_fraction_for_type_at_q(actor_type t, Resource q) {
    if(impact_aware) {
        if(q.total_actors_at_q) {
            return (q.actors_of_type[t]+1)/(q.total_actors_at_q+1);
        }
        else return 0;
    }
    else {
        if(q.total_actors_at_q) {
            if(q.total_actors_at_q == 1 && q.actors_of_type[t] == 1) return 1;
            return q.actors_of_type[t]/q.total_actors_at_q;
        }
        else return 0;
    }
}

function_space Simulator::get_cur_util_for_actor(const Actor& a, const bool measure_segregation) {
    if(a.cur_resource==-1) return -1;
    if(!impact_aware) return get_util_for_type_at_q(a.type,resources[a.cur_resource],utility_function,measure_segregation);
    else {
        if(measure_segregation) return (resources[a.cur_resource].total_actors_at_q)? (resources[a.cur_resource].total_actors_at_q-resources[a.cur_resource].actors_of_type[a.type])/(resources[a.cur_resource].total_actors_at_q): 0;
        else return utility_function((resources[a.cur_resource].actors_of_type[a.type])/(resources[a.cur_resource].total_actors_at_q));
    }
}

bool Simulator::swap_resource_for_actor(Actor& a) {

    function_space max_util = get_cur_util_for_actor(a);
    int max_q_idx = a.cur_resource;
    bool starts_before_peak = (a.cur_resource!=-1)? (resources[a.cur_resource].actors_of_type[a.type]/resources[a.cur_resource].total_actors_at_q) <= peak: 0;
    for(auto q_idx:a.available_resources) {
        if(
        resource_q_available(resources[q_idx]) &&
        q_idx != a.cur_resource &&
        get_util_for_type_at_q(a.type,resources[q_idx],utility_function) > max_util &&
        (a.cur_resource==-1 || allowed_swap_configs[starts_before_peak][get_fraction_for_type_at_q(a.type,resources[q_idx])<=peak])
        ){
            clog << "did" << starts_before_peak << "to " << (get_fraction_for_type_at_q(a.type,resources[q_idx])<=peak) << "jump. init: " << (a.cur_resource==-1) << endl;
            clog << "got util: " << get_util_for_type_at_q(a.type,resources[q_idx],utility_function) << " from " << q_idx << endl;
            max_util = get_util_for_type_at_q(a.type,resources[q_idx],utility_function);
            max_q_idx = q_idx;
            }
        }
    if(a.cur_resource != -1)clog << "gain: " << (max_util-get_cur_util_for_actor(a)) << "for min_improvement " << min_improvement << endl;
    if(max_q_idx != a.cur_resource && (max_util-get_cur_util_for_actor(a)) > min_improvement + numeric_limits<function_space>::epsilon()) {       //also handles case in which no adjacent resource has slots left and updates resources fractions
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
    for(Actor a: actors) res += get_util_for_type_at_q(a.type, resources[a.cur_resource],utility_function,true);
    return res;
}

int Simulator::step() {
    int swaps_made = 0;
    function_space utility_pre_swap = 0;
    for(auto& a:actors) {
        utility_pre_swap = get_total_utility(utility_function);
        swaps_made += swap_resource_for_actor(a);
        clog << "utility change caused by swap: " << get_total_utility(utility_function)-utility_pre_swap << endl;
    }
    clog << "swaps made in step: " << swaps_made << endl;
    return swaps_made;
}

//returns step at which nash equil was reached or -1
int Simulator::run_simulation(int steps, int data_collection_interval, vector<function_space>& social_welfare_at_collection_step, vector<vector<vector<function_space>>>& fraction_at_q_at_collection_step_for_type, vector<function_space>& segregation_welfare_at_collection_step) {
    fraction_at_q_at_collection_step_for_type.resize(resources.size());
    for(int i=0;i<steps;i++) {
        if(!step()) {
            cout<< "nash equilibrium reached in step " << i << endl;
            return i+1;
        }
        if(i%data_collection_interval==0) {
            social_welfare_at_collection_step.push_back(get_total_utility(utility_function));
            segregation_welfare_at_collection_step.push_back(get_total_segregation_welfare([](int x){return x;}));
            for(int q_idx=0;q_idx<resources.size();q_idx++) {
                fraction_at_q_at_collection_step_for_type[q_idx].resize(fraction_at_q_at_collection_step_for_type[q_idx].size()+1);
                for(int t=0; t<resources[q_idx].nr_of_types; t++) {
                    if (resources[q_idx].total_actors_at_q) fraction_at_q_at_collection_step_for_type[q_idx][i/data_collection_interval].push_back(resources[q_idx].actors_of_type[t]/resources[q_idx].total_actors_at_q);
                    else fraction_at_q_at_collection_step_for_type[q_idx][i/data_collection_interval].push_back(0);
                }
            }
        }
    }
    return -1;
}

Simulator::Simulator(vector<Actor> actors, vector<Resource> resources,
                     function<function_space(function_space)> utility_function, const function_space peak, const function_space epsilon, const bool isolation_optimal, const bool impact_aware, const vector<vector<bool>> allowed_swap_configs):
        actors(actors), resources(resources), utility_function(utility_function), peak(peak), min_improvement(epsilon), empty_neighbourhood_optimal(isolation_optimal), impact_aware(impact_aware), allowed_swap_configs(allowed_swap_configs){
}
