#ifndef RESSOURCE_SELECTION_SIMULATOR_UTIL_H
#define RESSOURCE_SELECTION_SIMULATOR_UTIL_H

#include "actor.h"
#include "Resource.h"
#include <iostream>
#include <random>

using namespace std;
//root is always actor
void bin_tree_game_construction(vector<Actor>& actors, vector<Resource>& resources, const int tree_height, const int nr_of_types);

#endif //RESSOURCE_SELECTION_SIMULATOR_UTIL_H
