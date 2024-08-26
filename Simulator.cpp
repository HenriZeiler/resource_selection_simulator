//
// Created by Henri Zeiler on 12.05.24.
//

#include "Simulator.h"
#include <iostream>
#include <sstream>
#include <fstream>

void Simulator::game_from_text() {  //expects all actors to be at some resource
    vector<pair<int,int>> color_cur_res;
    cout << "enter num of resources: " << endl;
    int nr_of_resources; cin >> nr_of_resources;
    for(int i=0;i<nr_of_resources;i++) {
        resources.push_back(Resource(10000,2));
        int red,blue;
        cout << "enter balance at resource " << i << endl;
        cin >> red >> blue;
        for(int j=0;j<red;j++) color_cur_res.push_back(pair(0,i));
        for(int j=0;j<blue;j++) color_cur_res.push_back(pair(1,i));
    }
    cin.ignore(1,'\n');
    for(int i=0; i<color_cur_res.size();i++) {
        string s;
        getline(cin,s);
        std::istringstream ss(s);
        actors.push_back(Actor(color_cur_res[i].first,vector(std::istream_iterator<int>{ss}, std::istream_iterator<int>{})));
        actors[i].cur_resource=color_cur_res[i].second;
        resources[color_cur_res[i].second].actors_of_type[color_cur_res[i].first]++;
        resources[color_cur_res[i].second].total_actors_at_q++;
    }
}

void Simulator::game_from_save() {  //expects all actors to be at some resource
    cout << "enter num of resources: " << endl;
    int nr_of_resources,nr_of_actors; cin >> nr_of_resources;
    for(int i=0;i<nr_of_resources;i++) {
        resources.push_back(Resource(10000,2));
    }
    cout << "enter number of actors: " << endl;
    cin >> nr_of_actors;
    while(nr_of_actors--) {
        int type, cur_resource; cin >> type >> cur_resource;
        string s;
        cin.ignore(1,'\n');
        getline(cin,s);
        istringstream ss(s);
        actors.push_back(Actor(type,vector(std::istream_iterator<int>{ss}, std::istream_iterator<int>{}),cur_resource));
        resources[cur_resource].actors_of_type[type]++;
        resources[cur_resource].total_actors_at_q++;
    }
}

void Simulator::game_to_textfile(const vector<Actor> actors, const vector<Resource> resources) {
    ofstream output;
    output.open("game_setup.txt");
    output << resources.size() << endl << actors.size() << endl;
    for(auto a: actors) {
        output << a.type << " " << a.cur_resource << endl;
        for(auto q: a.available_resources) output << q << " ";
        output << endl;
    }
    output.close();
}

bool Simulator::resource_q_available(const Resource q) {
    return q.max_slots > q.total_actors_at_q;
}

function_space Simulator::get_util_for_type_at_q(actor_type t, Resource q, function<function_space(function_space)> custom_util, const bool measure_segregation) {
    //TODO fix measure_segregation
    if(impact_aware) {
        if(measure_segregation) return (q.total_actors_at_q-q.actors_of_type[t])/(q.total_actors_at_q+1);
        else return custom_util((q.actors_of_type[t]+1)/(q.total_actors_at_q+1));
    }
    else {
        if(q.total_actors_at_q) {
            if(measure_segregation) return (q.total_actors_at_q-q.actors_of_type[t])/q.total_actors_at_q;
            else return custom_util(q.actors_of_type[t]/q.total_actors_at_q);
        }
        else return empty_neighbourhood_optimal*utility_function(peak);
    }
}

double_t Simulator::get_fraction_for_type_at_q(actor_type t, Resource q) {
    if(impact_aware) {
        return (q.actors_of_type[t]+1)/(q.total_actors_at_q+1);
    }
    else {
        if(q.total_actors_at_q) {
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
    bool starts_before_peak = (a.cur_resource!=-1 && resources[a.cur_resource].actors_of_type[a.type]/resources[a.cur_resource].total_actors_at_q != peak)? (resources[a.cur_resource].actors_of_type[a.type]/resources[a.cur_resource].total_actors_at_q) < peak: -1;
    for(auto q_idx:a.available_resources) {
        if(
        resource_q_available(resources[q_idx]) &&
        q_idx != a.cur_resource &&
        (get_util_for_type_at_q(a.type,resources[q_idx],utility_function) > max_util)
        //&& a.cur_resource==-1 || (allowed_swap_configs[starts_before_peak][get_fraction_for_type_at_q(a.type,resources[q_idx])<peak])
        ){
            //cout << "did" << starts_before_peak << "to " << (get_fraction_for_type_at_q(a.type,resources[q_idx])<=peak) << "jump. init: " << (a.cur_resource==-1) << endl;
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
        if(a.cur_resource != -1) res += get_util_for_type_at_q(a.type, resources[a.cur_resource],utility_function);
    }
    return res;
}

function_space Simulator::get_total_segregation_welfare(function<function_space(function_space)> utility_function) {
    function_space res = 0;
    for(Actor a: actors) res += get_util_for_type_at_q(a.type, resources[a.cur_resource],utility_function,true);
    return res;
}

double_t Simulator::get_average_fraction() {
    double_t acc = 0; int c = 0;
    for(Resource q: resources) {
        if(q.total_actors_at_q) {
            acc+=q.actors_of_type[1]*q.actors_of_type[1]/q.total_actors_at_q+q.actors_of_type[0]*q.actors_of_type[0]/q.total_actors_at_q; c++;
        }
    }
    return acc;
}

double_t Simulator::get_fraction_dif() {
    double_t acc = 0;
    for(int i = 0;i < resources.size();i++) {
        for (int j = 0; j < resources.size(); j++) {
            if (i != j && resources[i].actors_of_type[0] < resources[i].actors_of_type[1] &&
                resources[j].actors_of_type[0] < resources[j].actors_of_type[1])
                acc += abs(resources[i].actors_of_type[0] / resources[i].total_actors_at_q -
                           resources[j].actors_of_type[0] / resources[j].total_actors_at_q);
        }
    }
    return acc;
}
bool Simulator::check_if_v_in_range_around_q_for_type(Resource v, Resource q, int type) {
    //differentiate between red and blue jumps if necessary
    return ((q.actors_of_type[type]/(q.total_actors_at_q+1))<=v.actors_of_type[type]/v.total_actors_at_q &&
            (v.actors_of_type[type]/v.total_actors_at_q)<=(q.actors_of_type[type]+1)/(q.total_actors_at_q+1));
}

int Simulator::get_num_of_resources_in_range() {
    double_t acc = 0;
    for(int i=0;i<resources.size();i++) {
        for(int j = 0;j<resources.size();j++) {
            if (i != j && (check_if_v_in_range_around_q_for_type(resources[i], resources[j], 0) ||
                           (((resources[i].actors_of_type[0] / resources[i].total_actors_at_q) < peak) !=
                            (resources[j].actors_of_type[0] / resources[j].total_actors_at_q < peak) ||
                            (resources[i].actors_of_type[0] / resources[i].total_actors_at_q == peak)))) {
                acc++;
            } else {
                //cout << "not in range: " << i << " " << j << endl;
            }
        }
    }
    return acc;
}

int Simulator::step() {
    int swaps_made = 0;
    function_space utility_pre_swap = 0;
    for(auto& a:actors) {
        if(a.cur_resource!= -1) string s = "actors current resource before: " + to_string(resources[a.cur_resource].actors_of_type[0]) + " " + to_string(resources[a.cur_resource].actors_of_type[1]) + "\n";
        auto& res_pre = resources[a.cur_resource];
        utility_pre_swap = get_total_utility(utility_function);
        swaps_made += swap_resource_for_actor(a);
        /*if(get_total_utility(utility_function)-utility_pre_swap<0) {
            if(res_pre.actors_of_type[a.type]+1/res_pre.total_actors_at_q<peak && resources[a.cur_resource].actors_of_type[a.type]/resources[a.cur_resource].total_actors_at_q<peak
                || res_pre.actors_of_type[a.type]+1/res_pre.total_actors_at_q>peak && resources[a.cur_resource].actors_of_type[a.type]/resources[a.cur_resource].total_actors_at_q>peak) {
                if(0<res_pre.actors_of_type[(a.type+1)%2]*res_pre.actors_of_type[(a.type+1)%2]/(res_pre.total_actors_at_q*(res_pre.total_actors_at_q-1))-
                resources[a.cur_resource].actors_of_type[(a.type+1)%2]*resources[a.cur_resource].actors_of_type[(a.type+1)%2]/(resources[a.cur_resource].total_actors_at_q*(resources[a.cur_resource].total_actors_at_q-1))) {
                    cout << s;
                    cout << "fractional dif: " << res_pre.actors_of_type[(a.type+1)%2]*res_pre.actors_of_type[(a.type+1)%2]/(res_pre.total_actors_at_q*(res_pre.total_actors_at_q+1))-
                                                  resources[a.cur_resource].actors_of_type[(a.type+1)%2]*resources[a.cur_resource].actors_of_type[(a.type+1)%2]/(resources[a.cur_resource].total_actors_at_q*(resources[a.cur_resource].total_actors_at_q-1)) << endl;
                    cout << "actors current resource after: " << resources[a.cur_resource].actors_of_type[0] << " " << resources[a.cur_resource].actors_of_type[1] << endl;
                    exit(0);
                }
            }
        }*/
    }
    for(Resource q: resources) {
        cout << q.actors_of_type[0] << "|" << q.actors_of_type[1] << ", ";
    }
    cout << endl;
    //cout << endl << "avg_fraction: " <<get_fraction_dif() << endl;
    clog << "swaps made in step: " << swaps_made << endl;
    //cout << endl << get_num_of_resources_in_range() << endl;
    return swaps_made;
}

//returns step at which nash equil was reached or -1
int Simulator::run_simulation(int steps, int data_collection_interval, vector<function_space>& social_welfare_at_collection_step, vector<vector<vector<function_space>>>& fraction_at_q_at_collection_step_for_type, vector<function_space>& segregation_welfare_at_collection_step) {
    fraction_at_q_at_collection_step_for_type.resize(resources.size());
    resources[0].max_slots=INF;
    game_to_textfile(actors,resources);
    for(Resource q: resources) {
        cout << q.actors_of_type[0] << "|" << q.actors_of_type[1] << ", ";
    }
    cout << endl;

    for(int i=0;i<steps;i++) {
        if(i>0) {
            //allowed_swap_configs[1][0] = false;
            //allowed_swap_configs[0][1] = false;
        }
        int jumps_done = step();
        if (jumps_done && i % data_collection_interval == 0) {
            social_welfare_at_collection_step.push_back(get_total_utility(utility_function));
            //segregation_welfare_at_collection_step.push_back(get_total_segregation_welfare([](int x){return x;}));
            for (int q_idx = 0; q_idx < resources.size(); q_idx++) {
                fraction_at_q_at_collection_step_for_type[q_idx].resize(
                        fraction_at_q_at_collection_step_for_type[q_idx].size() + 1);
                for (int t = 0; t < resources[q_idx].nr_of_types; t++) {
                    if (resources[q_idx].total_actors_at_q)
                        fraction_at_q_at_collection_step_for_type[q_idx][i / data_collection_interval].push_back(
                                resources[q_idx].actors_of_type[t] / resources[q_idx].total_actors_at_q);
                    else
                        fraction_at_q_at_collection_step_for_type[q_idx][i / data_collection_interval].push_back(0);
                }
            }
        } else if(!jumps_done) {
            cout<< "nash equilibrium reached in step " << i << endl;
            return i+1;
        }
    }
    return -1;
}

Simulator::Simulator(vector<Actor> actors, vector<Resource> resources,
                     function<function_space(function_space)> utility_function, const function_space peak, const function_space epsilon, const bool isolation_optimal, const bool impact_aware, vector<vector<bool>> allowed_swap_configs):
        actors(actors), resources(resources), utility_function(utility_function), peak(peak), min_improvement(epsilon), empty_neighbourhood_optimal(isolation_optimal), impact_aware(impact_aware), allowed_swap_configs(allowed_swap_configs){
}
