#include "cell.h"

std::vector<std::vector<bool>> make_random(int x, int y, float density){
  std::vector<bool> temp_line(y,false);
  std::vector<std::vector<bool>> result;
  std::srand(std::time(nullptr));
  for(int j = 0; j < x; j++){
    for(int i = 0; i < y; i++){
      temp_line[i] = static_cast<float>(rand()) / static_cast<float>(RAND_MAX) < density;
    }
    result.push_back(temp_line);
  }
  return result;
}

void get_input(){
  Vector2 mouse_pos = GetMousePosition();
  float mouse_d = GetMouseWheelMove();
  global_variables.scale += 0.5 * global_variables.scale * mouse_d;
  global_variables.center_x += mouse_pos.x / 2;
  global_variables.center_y += mouse_pos.y / 2;
}

int sum_neighbors(int x, int y) {
  std::vector<std::vector<bool>>* canvas = (global_variables.timer%2) == 0 ? &global_variables.ping : &global_variables.pong;
     int result = 0;
    for (int dx = -1; dx <= 1; dx++)
        for (int dy = -1; dy <= 1; dy++)
            if (!(dx == 0 && dy == 0)) {
                int nx = x + dx;
                int ny = y + dy;
                if (nx >= 0 && ny >= 0 &&
                    nx < canvas->size() &&
                    ny < (*canvas)[0].size())
                    result += (*canvas)[nx][ny];
            }
    return result;
}

void add_neighbors(int x, int y) {
  std::vector<std::vector<bool>>* canvas = (global_variables.timer%2) == 0 ? &global_variables.ping : &global_variables.pong;
    for (int dx = -1; dx <= 1; dx++) {
        for (int dy = -1; dy <= 1; dy++) {
            if (dx == 0 && dy == 0) continue; // Skip the center cell

            int nx = x + dx;
            int ny = y + dy;

            // Optional bounds check:
            if (nx >= 0 && ny >= 0 &&
                nx < canvas->size() &&
                ny < (*canvas)[0].size()) {
                
                std::pair<int, int> neighbor = {nx, ny};

                // Only insert if not already present
                if (global_variables.active.find(neighbor) == global_variables.active.end()) {
                    global_variables.active[neighbor] = 0;
                }
            }
        }
    }
}

void update(){
  global_variables.timer++;
  std::vector<std::vector<bool>>* ping = 
    (global_variables.timer%2) == 0 ? &global_variables.ping : &global_variables.pong;
  std::vector<std::vector<bool>>* pong = 
    (global_variables.timer%2) == 1 ? &global_variables.ping : &global_variables.pong;
  std::vector<std::pair<int, int>> erase_list;
  int neighbor_sum = 0;
  int x = 0;
  int y = 0;
  int max_x = global_variables.gui_width;
  int max_y = global_variables.gui_height;
  for(auto& element : global_variables.active){
    if(element.first.first > max_x - 1 || element.first.second > max_y - 1 || element.first.first < 2 || element.first.second < 2) continue;
    if(element.second > global_variables.max_age)
      erase_list.push_back(element.first);
    x = element.first.first;
    y = element.first.second;
    neighbor_sum = sum_neighbors(x, y);
    if(neighbor_sum == 3) {
      if(!(*pong)[y][x]){
        add_neighbors(x,y);
        (*pong)[y][x] = true;
        global_variables.active[element.first] = 0;
      } else {
        global_variables.active[element.first]++;
      }
    }
    else if(neighbor_sum == 2) global_variables.active[element.first]++;
    else {
      global_variables.active[element.first] = 0;
      (*pong)[y][x] = false;
    }
  }
  for(std::pair victim : erase_list){
    global_variables.active.erase(victim);
  }
}

void display() {
    std::vector<std::vector<bool>>* canvas = (global_variables.timer%2) == 0 ? &global_variables.ping : &global_variables.pong; 
    BeginDrawing();
    for (const auto& [coord, age] : global_variables.active) {
        int x = coord.first;
        int y = coord.second;
        if (y >= 0 && y < global_variables.gui_height &&
            x >= 0 && x < global_variables.gui_width) {

            if ((*canvas)[x][y]) {
                int green = std::max(100, 255 - 20 * age);
                DrawPixel(x, y, {0, (unsigned char)green, 0, 255});
            } else {
                DrawPixel(x, y, BLACK);
            }
        }
    }
    EndDrawing();
}

void initial_pass(){
  std::vector<std::vector<bool>>* canvas = (global_variables.timer%2) == 0 ? &global_variables.ping : &global_variables.pong; 
  int y = canvas->size();
  int x = (*canvas)[0].size();
  for(int i = 0; i < y; i++){
    for(int j = 0; j < x; j++){
      if((*canvas)[j][i] == true){
        add_neighbors(j, i);
      }
    }
  }
}
