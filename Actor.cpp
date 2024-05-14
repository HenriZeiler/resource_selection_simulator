//
// Created by Henri Zeiler on 12.05.24.
//


#include "Actor.h"

Actor::Actor(const actor_type type, vector<int> available_resources) :type(type),available_resources(available_resources) {
    cur_resource = -1;
}

