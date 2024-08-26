#ifndef RESSOURCE_SELECTION_SIMULATOR_UTIL_H
#define RESSOURCE_SELECTION_SIMULATOR_UTIL_H

#include "Actor.h"
#include "Resource.h"
#include <iostream>
#include <random>

using namespace std;
//root is always actor
void bin_tree_game_construction(vector<Actor>& actors, vector<Resource>& resources, const int tree_height, const int nr_of_types, const bool first_level_is_actor);
void four_grid_torus_game_construction(vector<Actor>& actors, vector<Resource>& resources, const int grid_height, const int grid_width, const int nr_of_types, bool enforce_diagonals_for_actors=true);
void complete_graph_construction(vector<Actor>& actors, vector<Resource>& resources, const int nr_of_resources, const int nr_of_actors, const int nr_of_types);
void random_graph_construction(vector<Actor>& actors, vector<Resource>& resources, const int nr_of_resources, const int nr_of_actors, const int nr_of_types);
void uniformly_distribute_actors(vector<Actor>& actors, vector<Resource>& resources);
void segregate_actors(vector<Actor>& actors, vector<Resource>& resources);
void distribute_actors_over_adjacent_resources(vector<Actor>& actors, vector<Resource>& resources); //assumes no actors are already at resources
#endif //RESSOURCE_SELECTION_SIMULATOR_UTIL_H
