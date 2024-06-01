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
    function_space peak = 0.25;
    auto utility_fct = [peak](function_space x){return (x<=peak)? (x): (peak*(1-x)/(1-peak));};
    //-------------------------------
    //std::clog.setstate(std::ios_base::failbit); //disable logging

    vector<Actor> actors;
    vector<Resource> resources;
    vector<int> tmp;

    //IAE Counterexample
    for(int i=0;i<3;i++) resources.push_back(Resource(1000,nr_of_types));
    for(int i=0;i<4;i++) actors.push_back(Actor(0,vector{0}));
    for(int i=0;i<7;i++) actors.push_back(Actor(1,vector{0}));
    for(int i=0;i<5;i++) actors.push_back(Actor(0,vector{2}));
    for(int i=0;i<1;i++) actors.push_back(Actor(1,vector{2}));
    actors.push_back(Actor(0,vector{0,1}));
    actors.push_back(Actor(1,vector{1}));
    actors.push_back(Actor(1,vector{1,2}));

    Simulator sim = Simulator(actors,resources,utility_fct,0,true);

    vector<function_space> social_welfare_at_collection_step;
    vector<vector<vector<function_space>>> fraction_at_q_at_collection_step_for_type;
    vector<function_space> segregation_welfare_at_collection_step;
    int equilibrium = sim.run_simulation(sim_steps,collection_data_interval,social_welfare_at_collection_step,fraction_at_q_at_collection_step_for_type,segregation_welfare_at_collection_step);
    if(equilibrium != -1) visualize_fractions(equilibrium/collection_data_interval+1,resources.size(),fraction_at_q_at_collection_step_for_type);
    else visualize_fractions(sim_steps/collection_data_interval+1,resources.size(),fraction_at_q_at_collection_step_for_type);
    return 0;
}
