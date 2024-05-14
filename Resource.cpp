//
// Created by Henri Zeiler on 12.05.24.
//

#include "Resource.h"

Resource::Resource(const int max_slots, const int nr_of_types) : max_slots(max_slots) {
    fractions_of_types = vector<double_t>(nr_of_types);
}