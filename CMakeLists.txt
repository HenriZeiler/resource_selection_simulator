cmake_minimum_required(VERSION 3.29)
project(ressource_selection_simulator)

set(CMAKE_CXX_STANDARD 20)
#find_package(matplot REQUIRED)

include_directories(/opt/homebrew/include)
link_directories(/opt/homebrew/lib)

add_executable(ressource_selection_simulator main.cpp Simulator.cpp Simulator.h Actor.cpp Actor.h Resource.cpp Resource.h model.h visualize.cpp visualize.h util.cpp util.h)
target_link_libraries(ressource_selection_simulator PUBLIC matplot)