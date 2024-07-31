//
// Created by Henri Zeiler on 15.05.24.
//

#ifndef RESSOURCE_SELECTION_SIMULATOR_VISUALIZE_H
#define RESSOURCE_SELECTION_SIMULATOR_VISUALIZE_H

#include "model.h"
#include <vector>


void visualize_fractions(int sim_steps,int nr_of_resources,vector<vector<vector<function_space>>>& fraction_at_q_at_collection_step_for_type);
void test();
#endif //RESSOURCE_SELECTION_SIMULATOR_VISUALIZE_H
