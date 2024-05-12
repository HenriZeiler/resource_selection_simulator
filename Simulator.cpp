//
// Created by Henri Zeiler on 12.05.24.
//

#include "Simulator.h"

Simulator::Simulator(const int number_of_types, vector<Actor> actors, vector<Resource> resources,
                     function<function_space(function_space)> utility_function) :
                     number_of_types(number_of_types), actors(actors), resources(resources), utility_function(utility_function) {
}