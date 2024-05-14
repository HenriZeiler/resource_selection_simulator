//
// Created by Henri Zeiler on 12.05.24.
//
#ifndef RESSOURCE_SELECTION_SIMULATOR_SIMULATOR_H
#define RESSOURCE_SELECTION_SIMULATOR_SIMULATOR_H



#include <cstdlib>
#include <vector>
#include <functional>
#include "Resource.h"
#include "Actor.h"
#include "util.h"

using namespace std;

typedef class Simulator {
private:
    vector<Actor> actors;
    vector<Resource> resources;
    function<function_space(function_space)> utility_function;

    int step();     //returns number of swaps in step
    void run_simulation(int steps = 10);
    bool check_for_nash_equilibrium();
    double get_segregation_welfare();
    bool resource_q_available(int q_idx);
public:
    const int number_of_types;
    Simulator(const int number_of_types, vector<Actor> actors, vector<Resource> resources, function<function_space(function_space)> utility_function);
} Simulator;


#endif //RESSOURCE_SELECTION_SIMULATOR_SIMULATOR_H
