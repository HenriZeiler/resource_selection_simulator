#include <iostream>

#include "Simulator.h"
#include "Actor.h"
#include "Resource.h"
#include "util.h"

int main() {
    vector<Actor> actors;
    actors.push_back(Actor(1,{1,2}));
    actors.push_back(Actor(2,{2}));
    vector<Resource> resources;
    resources.push_back(Resource(2,2));
    resources.push_back(Resource(2,2));
    Simulator sim = Simulator(actors,resources,[](function_space x){return 1-x;});

    vector<function_space> social_welfare_at_collection_step;
    vector<vector<function_space>> fraction_at_q_at_collection_step;
    vector<function_space> segregation_welfare_at_collection_step;
    sim.run_simulation(5,1,social_welfare_at_collection_step,fraction_at_q_at_collection_step,segregation_welfare_at_collection_step);
    return 0;
}
