//
// Created by Henri Zeiler on 12.05.24.
//

#include "Resource.h"

unordered_set<int> Resource::get_actors_at_q() {return this->actors_at_q;}

int Resource::get_max_slots() {return this->max_slots;}

void Resource::join_resource(int a) {this->actors_at_q.insert(a);}

void Resource::leave_resource(int a) {this->actors_at_q.erase(a);}

void Resource::set_max_slots(int v) {this->max_slots = v;}

Resource::Resource(const int idx, const int max_slots, const int nr_of_types) : idx(idx), max_slots(max_slots) {
    this->fractions_of_types = vector<double_t>(nr_of_types);
}