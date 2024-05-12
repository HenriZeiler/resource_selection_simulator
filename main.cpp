#include <iostream>
#include "Actor.h"

int main() {
    std::cout << "Hello, World!" << std::endl;
    vector<int> v = {1,2};
    Actor* a = new Actor(1,1,v);
    return 0;
}
