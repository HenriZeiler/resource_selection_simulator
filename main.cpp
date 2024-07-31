#include <iostream>

#include "Simulator.h"
#include "Actor.h"
#include "Resource.h"
#include "model.h"
#include "visualize.h"
#include "util.h"


int main() {
    //-----Simulation Parameters-----
    int sample_count = 3;
    int sim_steps = 10;
    int collection_data_interval = 1;
    int nr_of_types = 2;
    function_space peak = 0.6;
    auto utility_fct = [peak](function_space x){return (x<=peak)? (x): (peak*(1-x)/(1-peak));};
    vector<vector<bool>> allowed_swap_configs = {{0,1},{0,0}};
    //-------------------------------
    //std::clog.setstate(std::ios_base::failbit); //disable logging

    //IAE Counterexample
    /*for(int i=0;i<3;i++) resources.push_back(Resource(1000,nr_of_types));
    for(int i=0;i<4;i++) actors.push_back(Actor(0,vector{0}));
    for(int i=0;i<7;i++) actors.push_back(Actor(1,vector{0}));
    for(int i=0;i<5;i++) actors.push_back(Actor(0,vector{2}));
    for(int i=0;i<1;i++) actors.push_back(Actor(1,vector{2}));
    actors.push_back(Actor(0,vector{0,1}));
    actors.push_back(Actor(1,vector{1}));
    actors.push_back(Actor(1,vector{1,2}));*/

    //behind to before jumps only counterexample
    /*for(int i=0;i<2;i++) resources.push_back(Resource(1000,nr_of_types));
    for(int i=0;i<3;i++) actors.push_back(Actor(0,vector{0}));
    actors.push_back(Actor(0,vector{0,1}));
    for(int i=0;i<12;i++) actors.push_back(Actor(1,vector{0}));
    for(int i=0;i<1;i++) actors.push_back(Actor(0,vector{1}));
    for(int i=0;i<4;i++) actors.push_back(Actor(1,vector{1}));*/
    for(int i = 0;i<sample_count;i++) {
        vector<Actor> actors;
        vector<Resource> resources;

        bin_tree_game_construction(actors, resources, 7, nr_of_types);

        Simulator sim = Simulator(actors, resources, utility_fct, peak, 0, false, false);
        cout << resources.size();
        vector<function_space> social_welfare_at_collection_step;
        vector<vector<vector<function_space>>> fraction_at_q_at_collection_step_for_type;
        vector<function_space> segregation_welfare_at_collection_step;
        int equilibrium = sim.run_simulation(
                sim_steps,
                collection_data_interval,
                social_welfare_at_collection_step,
                fraction_at_q_at_collection_step_for_type,
                segregation_welfare_at_collection_step);
        if (equilibrium != -1) visualize_fractions(
                equilibrium/collection_data_interval+1,
                resources.size(),
                fraction_at_q_at_collection_step_for_type);
        else {
            visualize_fractions(
                    sim_steps / collection_data_interval + 1,
                    resources.size(),
                    fraction_at_q_at_collection_step_for_type);
            return 0;
        }
    }
    return 0;
}
