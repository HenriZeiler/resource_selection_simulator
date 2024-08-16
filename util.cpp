//
// Created by Henri Zeiler on 31.07.24.
//

#include "util.h"
#include "Actor.h"
#include "Resource.h"
#include <iostream>
#include <random>

void bin_tree_game_construction(vector<Actor>& actors, vector<Resource>& resources, const int tree_height, const int nr_of_types, const bool first_level_is_actor) {
    if(tree_height<2) {
        cerr << "invalid tree height of " << tree_height << " passed to bin_tree_game_construction()" << endl;
        return;
    }
    std::random_device dev;
    std::mt19937 rng(dev());
    std::uniform_int_distribution<std::mt19937::result_type> dist1(0,1);
    if(first_level_is_actor) {
        for (int i = 1; i < tree_height; i += 2) {
            for (int j = 0; j < pow(2, i); j++) {
                resources.push_back(Resource(INF, nr_of_types));
            }
        }
        actors.push_back(Actor(dist1(rng), {0, 1}));
        int count = 1;
        for (int i = 2; i < tree_height; i += 2) {
            for (int j = 0; j < pow(2, i); j++, count++) {
                if (tree_height % 2 == 0 || i < tree_height - 1)
                    actors.push_back(Actor(dist1(rng), {(count + 1) / 2 - 1, 2 * count, 2 * count + 1}));
                else actors.push_back(Actor(dist1(rng), {(count + 1) / 2 - 1}));
            }
        }
    } else {
        for (int i = 0; i < tree_height; i += 2) {
            for (int j = 0; j < pow(2, i); j++) {
                resources.push_back(Resource(INF, nr_of_types));
            }
        }
        int count = 0;
        for (int i = 1; i < tree_height; i += 2) {
            for (int j = 0; j < pow(2, i); j++, count++) {
                if (tree_height % 2 == 1 || i < tree_height - 1)
                    actors.push_back(Actor(dist1(rng), {count/2, 2 * count + 1, 2 * count + 2}));
                else actors.push_back(Actor(dist1(rng), {count/2}));
            }
        }
    }
}

void four_grid_torus_game_construction(vector<Actor>& actors, vector<Resource>& resources, const int grid_height, const int grid_width, const int nr_of_types, bool enforce_diagonals_for_actors)   {
    std::random_device dev;
    std::mt19937 rng(dev());
    std::uniform_int_distribution<std::mt19937::result_type> dist1(0,1);
    if(enforce_diagonals_for_actors) {
        //start by labeling indeces with 0 for actor and 1 for resource
        vector<bool> node_type_at_index;
        node_type_at_index.push_back(0);
        for (int j = 1; j < grid_width; j++) {
            node_type_at_index.push_back(!node_type_at_index[(j-1)]);
        }
        for (int i = 1; i < grid_height; i++) {
            if(grid_width%2) {
                node_type_at_index.push_back(!node_type_at_index[(i-1)*grid_width+(grid_width-1)]);
            } else {
                node_type_at_index.push_back(node_type_at_index[(i-1)*grid_width+(grid_width-1)]);
            }
            for (int j = 1; j < grid_width; j++) {
                node_type_at_index.push_back(!node_type_at_index[i*grid_width+(j-1)]);
            }
        }
         for(int i=0;i<node_type_at_index.size();i++) {
            if(node_type_at_index[i]) {
                resources.push_back(Resource(INF,nr_of_types));
            } else {
                //find neighbours
                vector<int> adjacent_nodes;
                if((i+1)%grid_width==0) {   //right
                    adjacent_nodes.push_back(i-(grid_width-1));
                } else {
                    adjacent_nodes.push_back(i+1);
                }
                if(i%grid_width==0) {   //left
                    adjacent_nodes.push_back(i+(grid_width-1));
                } else {
                    adjacent_nodes.push_back(i-1);
                }
                if(i>=node_type_at_index.size()-grid_width) {          //down
                    adjacent_nodes.push_back((i-grid_width*(grid_height-1)));
                } else {
                    adjacent_nodes.push_back(i+grid_width);
                }
                if(i<grid_width) {   //up
                    adjacent_nodes.push_back((i+grid_width*(grid_height-1)));
                } else {
                    adjacent_nodes.push_back(i-grid_width);
                }
                //filter for resources
                erase_if(adjacent_nodes,[node_type_at_index](int i){return !node_type_at_index[i];});
                actors.push_back(Actor(dist1(rng),adjacent_nodes));
            }
        }
    } else {
        //TODO
    }
}