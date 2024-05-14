//
// Created by Henri Zeiler on 12.05.24.
//


#include "Actor.h"

Actor::Actor(const int idx, const actor_type type, vector<int> available_resources) :idx(idx),type(type),available_resources(available_resources) {
    this->cur_resource = -1;
}

int Actor::get_cur_resource() {
    return this->cur_resource;
}

actor_type Actor::get_type() {
    return this->type;
}

function_space Actor::get_util(function<function_space(function_space)> util_function) {
    return util_function(this->type);
}

bool Actor::switch_to_best_resource() {
    function_space best_util = -1;
    for(auto q: this->available_resources) {

    }
}