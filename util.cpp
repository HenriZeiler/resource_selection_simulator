//
// Created by Henri Zeiler on 31.07.24.
//

#include "util.h"
#include "Actor.h"
#include "Resource.h"
#include <iostream>
#include <random>
#include <map>

void uniformly_distribute_actors(vector<Actor>& actors, vector<Resource>& resources) {  //assumes no actors are already at resources
    for(int i=0;i<actors.size();i++) {
        actors[i].cur_resource = i%resources.size();
        resources[i%resources.size()].total_actors_at_q++;
        resources[i%resources.size()].actors_of_type[actors[i].type]++;
    }
}

void distribute_actors_over_adjacent_resources(vector<Actor>& actors, vector<Resource>& resources) {  //assumes no actors are already at resources
    std::random_device dev;
    std::mt19937 rng(dev());
    for(int i=0;i<actors.size();i++) {
        std::uniform_int_distribution<std::mt19937::result_type> dist1(0,actors[i].available_resources.size()-1);
            int assigned_resource =dist1(rng);
            int c=0;
            while(actors[i].available_resources.size() && c++ < 10) {
                if (resources[actors[i].available_resources[assigned_resource]].total_actors_at_q <
                    resources[actors[i].available_resources[assigned_resource]].max_slots) {
                    actors[i].cur_resource = actors[i].available_resources[assigned_resource];
                    resources[actors[i].available_resources[assigned_resource]].total_actors_at_q++;
                    resources[actors[i].available_resources[assigned_resource]].actors_of_type[actors[i].type]++;
                    break;
                } else {
                    assigned_resource = dist1(rng);
                }
            }
    }
}

void segregate_actors(vector<Actor>& actors, vector<Resource>& resources) {
    for(int i=0;i<actors.size();i++) {
        for(int c=0;c<resources.size()/2-1;c++) {
            if(resources[i % (resources.size() / 2) + ((resources.size() / 2) * actors[i].type)+c].max_slots>resources[i % (resources.size() / 2) + ((resources.size() / 2) * actors[i].type)+c].total_actors_at_q) {
                actors[i].cur_resource = i % (resources.size() / 2) + ((resources.size() / 2) * actors[i].type)+c;
                resources[i % (resources.size() / 2) + ((resources.size() / 2) * actors[i].type)+c].total_actors_at_q++;
                resources[i % (resources.size() / 2) +
                          ((resources.size() / 2) * actors[i].type)+c].actors_of_type[actors[i].type]++;
                break;
            }
        }
    }
}

void complete_graph_construction(vector<Actor>& actors, vector<Resource>& resources, const int nr_of_resources, const int nr_of_actors, const int nr_of_types) {
    std::random_device dev;
    std::mt19937 rng(dev());
    std::uniform_int_distribution<std::mt19937::result_type> dist1(0,2);
    vector<int> adj;
    for(int i=0;i<nr_of_resources;i++) {
        resources.push_back(Resource(INF,nr_of_types));
        adj.push_back(i);
    }
    for(int i=0;i<nr_of_actors;i++) {
        actors.push_back(Actor(dist1(rng)%2,adj));
    }
}

void random_graph_construction(vector<Actor>& actors, vector<Resource>& resources, const int nr_of_resources, const int nr_of_actors, const int nr_of_types) {
    std::random_device dev;
    std::mt19937 rng(dev());
    std::uniform_int_distribution<std::mt19937::result_type> dist1(0,1);
    vector<int> adj;
    for(int i=0;i<nr_of_resources;i++) {
        resources.push_back(Resource(10000,nr_of_types));
        adj.push_back(i);
    }
    resources[0].max_slots=10;
    for(int i=0;i<nr_of_actors;i++) {
        vector<int> tmp;
        for(int i:adj) {
            if(dist1(rng)) tmp.push_back(i);
        }
        actors.push_back(Actor(dist1(rng),tmp));
    }
}

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
        map<int,int> index_to_resource_index;
        int c=0;
         for(int i=0;i<node_type_at_index.size();i++) {
             if (node_type_at_index[i]) {
                 resources.push_back(Resource(INF, nr_of_types));
                 index_to_resource_index[i] = c++;
             }
         }
         for(int i=0;i<node_type_at_index.size();i++) {
             if(!node_type_at_index[i]) {
                 //find neighbours
                 vector<int> adjacent_nodes;
                 if ((i + 1) % grid_width == 0) {   //right
                     adjacent_nodes.push_back(i - (grid_width - 1));
                 } else {
                     adjacent_nodes.push_back(i + 1);
                 }
                 if (i % grid_width == 0) {   //left
                     adjacent_nodes.push_back(i + (grid_width - 1));
                 } else {
                     adjacent_nodes.push_back(i - 1);
                 }
                 if (i >= node_type_at_index.size() - grid_width) {          //down
                     adjacent_nodes.push_back((i - grid_width * (grid_height - 1)));
                 } else {
                     adjacent_nodes.push_back(i + grid_width);
                 }
                 if (i < grid_width) {   //up
                     adjacent_nodes.push_back((i + grid_width * (grid_height - 1)));
                 } else {
                     adjacent_nodes.push_back(i - grid_width);
                 }
                 //filter for resources
                 erase_if(adjacent_nodes, [node_type_at_index](int i) { return !node_type_at_index[i]; });
                 for (auto &a: adjacent_nodes) a = index_to_resource_index[a];
                 actors.push_back(Actor(dist1(rng), adjacent_nodes));
             }
        }
    } else {
        //TODO
    }
}

//vector<vector<pair<int,int>> compute_total_index_difference_for_jump