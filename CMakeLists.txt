cmake_minimum_required(VERSION 3.29)
project(ressource_selection_simulator)

set(CMAKE_CXX_STANDARD 20)

add_executable(ressource_selection_simulator main.cpp Simulator.cpp Simulator.h Actor.cpp Actor.h Resource.cpp Resource.h util.cpp util.h)
