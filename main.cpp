#include <iostream>

#include "Simulator.h"
#include "Actor.h"
#include "Resource.h"
#include "util.h"
#include "visualize.h"


int main() {
    //-----Simulation Parameters-----
    int sim_steps = 10;
    int collection_data_interval = 1;
    int nr_of_types = 2;
    auto utility_fct = [](function_space x){return x;};
    //-------------------------------
    //std::clog.setstate(std::ios_base::failbit); //disable logging

    /*vector<Actor> actors;
    actors.push_back(Actor(0,{0,1}));
    actors.push_back(Actor(1,{1}));
    vector<Resource> resources;
    resources.push_back(Resource(2,nr_of_types));
    resources.push_back(Resource(2,nr_of_types));
    Simulator sim = Simulator(actors,resources,[](function_space x){return 1-x;});*/

    vector<Actor> actors;
    vector<Resource> resources;
    vector<int> tmp;
    for(int i=0;i<10;i++) {
        actors.push_back(Actor(i%2,vector{max(0,i-1),i,min(i+1,9)}));
        resources.push_back(Resource(10,nr_of_types));
    }
    Simulator sim = Simulator(actors,resources,utility_fct,true);

    vector<function_space> social_welfare_at_collection_step;
    vector<vector<vector<function_space>>> fraction_at_q_at_collection_step_for_type;
    vector<function_space> segregation_welfare_at_collection_step;
    int equilibrium = sim.run_simulation(sim_steps,collection_data_interval,social_welfare_at_collection_step,fraction_at_q_at_collection_step_for_type,segregation_welfare_at_collection_step);
    if(equilibrium != -1) visualize_fractions(equilibrium/collection_data_interval+1,resources.size(),fraction_at_q_at_collection_step_for_type);
    else visualize_fractions(sim_steps/collection_data_interval+1,resources.size(),fraction_at_q_at_collection_step_for_type);
    return 0;
}
