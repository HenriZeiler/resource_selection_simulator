#include <iostream>

#include "Simulator.h"
#include "Actor.h"
#include "Resource.h"
#include "util.h"
#include <matplot/matplot.h>

using namespace matplot;
int main() {
    //-----Simulation Parameters-----
    int sim_steps = 5;
    int collection_data_interval = 1;
    int nr_of_types = 2;
    //-------------------------------

    vector<Actor> actors;
    actors.push_back(Actor(0,{0,1}));
    actors.push_back(Actor(1,{1}));
    vector<Resource> resources;
    resources.push_back(Resource(2,nr_of_types));
    resources.push_back(Resource(2,nr_of_types));
    Simulator sim = Simulator(actors,resources,[](function_space x){return 1-x;});

    vector<function_space> social_welfare_at_collection_step;
    vector<vector<vector<function_space>>> fraction_at_q_at_collection_step_for_type;
    vector<function_space> segregation_welfare_at_collection_step;
    sim.run_simulation(sim_steps,collection_data_interval,social_welfare_at_collection_step,fraction_at_q_at_collection_step_for_type,segregation_welfare_at_collection_step);
    tiledlayout(sim_steps,resources.size());
    for(int i=0;i<resources.size();i++) {
        for(int j=0;j<fraction_at_q_at_collection_step_for_type[0].size();j++) {
            auto ax_tmp = nexttile();
            pie(ax_tmp,fraction_at_q_at_collection_step_for_type[i][j]);
            title("Resource " + to_string(i) + " at step " + to_string(j));
        }
    }
    show();
    return 0;
}
