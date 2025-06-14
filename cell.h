#pragma once
#include "raylib.h"
#include <ctime>
#include <vector>
#include <cstdlib>
#include <map>
#include <string>
#include <iostream>

struct GlobalVariables {
    int gui_width = 1000; // width of the GUI
    int gui_height = 600; // height of the GUI
    int FPS = 60;         // target FPS
    int max_age = 10;     // age before cells are ignored
    int timer = 0;        // # of iterations since start

    float center_x = gui_width / 2.0;  // center of display
    float center_y = gui_height / 2.0; // center of display
    float scale = 1;     // scale of display
    float density = 0.5; // initial density of simulation

    /*	ping and pong are identical grids of squares
     *
     *  on even generations, ping is read, and the
     *  next generation is written on pong.
     *
     *  on odd generations, roles are reversed
     *
     *  most recently updated is used to update map,
     *  and to write to display
     * */

    std::vector<std::vector<bool>> ping;
    std::vector<std::vector<bool>> pong;

    /*  active is a list of active cells and their
     *  neighbors, that are younger than a certain age
     *
     *  once a cell has an age > max_age, it is removed 
     *  from active.
     *
     *  active is initialized once the round begins,
     *  using an exhaustive search that enters every
     *  living element and its neighbors
     * */
    std::map<std::pair<int, int>, int> active;
};

// creates an instance of GlobalVariables in the main file
extern GlobalVariables global_variables;

// creates a random(ish) 2d-vector of booleans, with a density
std::vector<std::vector<bool>> make_random(int x, int y, float density);

// mouse and keyboard input, directly updates the contents of
// global_variables
void get_input();

// returns the sum of all eight cells surrounding {x,y}
int sum_neighbors(int x, int y);

// adds the neighbors of {x,y} to global_variables.active,
// unless already present
void add_neighbors(int x, int y);

// steps the function forwards
void update();

// displays a portion of the grid
void display();

// once a fresh random grid has been made, this function is
// used to create a global_variables.active by adding every live
// cell and its neighbors to active
void initial_pass();
