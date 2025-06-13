#pragma once
#include "raylib.h"
#include <ctime>
#include <vector>
#include <cstdlib>
#include <map>
#include <string>
#include <iostream>

struct GlobalVariables {
    int gui_width = 1000;
    int gui_height = 600;
    int FPS = 60;
    int max_age = 10;
    int timer = 0;

    float center_x = gui_width / 2.0;
    float center_y = gui_height / 2.0;
    float scale = 1;
    float density = 0.5;

    std::vector<std::vector<bool>> ping;
    std::vector<std::vector<bool>> pong;
    std::map<std::pair<int, int>, int> active;
};

extern GlobalVariables global_variables;

std::vector<std::vector<bool>> make_random(int x, int y, float density);

void get_input();

int sum_neighbors(int x, int y);

void add_neighbors(int x, int y);

void update();

void display();

void initial_pass();
