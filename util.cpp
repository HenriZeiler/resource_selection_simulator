//
// Created by Henri Zeiler on 31.07.24.
//

#include "util.h"
#include "actor.h"
#include "Resource.h"
#include <iostream>
#include <random>

void bin_tree_game_construction(vector<Actor>& actors, vector<Resource>& resources, const int tree_height, const int nr_of_types) {
    if(tree_height<2) {
        cerr << "invalid tree height of " << tree_height << " passed to bin_tree_game_construction()" << endl;
        return;
    }
    std::random_device dev;
    std::mt19937 rng(dev());
    std::uniform_int_distribution<std::mt19937::result_type> dist1(0,1);
    for(int i=1;i<tree_height;i+=2) {
        for(int j=0;j<pow(2,i);j++) {
            resources.push_back(Resource(INF,nr_of_types));
        }
    }
    actors.push_back(Actor(dist1(rng),{0,1}));
    int count = 1;
    for(int i=2;i<tree_height;i+=2) {
        for(int j=0;j<pow(2,i);j++,count++) {
            if(tree_height%2==0 || i<tree_height-1) actors.push_back(Actor(dist1(rng),{(count+1)/2-1,2*count,2*count+1}));
            else actors.push_back(Actor(dist1(rng),{(count+1)/2-1}));
        }
    }
}