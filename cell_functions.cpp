#include "cell.h"

std::vector<std::vector<bool>> make_random(int width, int height, float density) {
  std::srand(static_cast<unsigned int>(std::time(nullptr))); // Seed RNG
  std::vector<std::vector<bool>> result(width, std::vector<bool>(height, false));
  for (int x = 0; x < width; ++x) {
    for (int y = 0; y < height; ++y) {
      float random_val = static_cast<float>(std::rand()) / static_cast<float>(RAND_MAX);
      result[x][y] = random_val < density;
    }
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
  for (int dx = -1; dx <= 1; dx++){
    for (int dy = -1; dy <= 1; dy++){
      if (!(dx == 0 && dy == 0)) {
        int nx = x + dx;
        int ny = y + dy;
        if (nx >= 0 && ny >= 0 &&
          nx < canvas->size() &&
          ny < (*canvas)[0].size())
          result += (*canvas)[nx][ny];
      }
    }
  }
  return result;
}

void add_neighbors(int x, int y) {
  std::vector<std::vector<bool>>* canvas = (global_variables.timer%2) == 0 ? &global_variables.ping : &global_variables.pong;
    for (int dx = -1; dx <= 1; dx++) {
        for (int dy = -1; dy <= 1; dy++) {
            if (dx == 0 && dy == 0) continue;
            int nx = x + dx;
            int ny = y + dy;
            if (nx >= 0 && ny >= 0 &&
                nx < canvas->size() &&
                ny < (*canvas)[0].size()) {
                std::pair<int, int> neighbor = {nx, ny};
                if (global_variables.active.find(neighbor) == global_variables.active.end()) {
                    global_variables.active[neighbor] = 0;
                }
            }
        }
    }
}

void update(){
  global_variables.timer++;

  // read is a pointer to either ping or pong, depending on timer
  std::vector<std::vector<bool>>* read = (global_variables.timer % 2)? &global_variables.ping : &global_variables.pong;
  std::vector<std::vector<bool>>* write = (global_variables.timer % 2)? &global_variables.pong : &global_variables.ping;
  // list of cells too old to bother simulating, will be
  // removed from global_variables.active once update is complete
  std::vector<std::pair<int, int>> erase_list;

  /*  Theory of operation:
   *    iterate through active
   *    for each cell in active, check neighborhood sum
   *    if that sum == 2, leave unchanged, increment age
   *    if that sum == 3, make alive if dead and set age == 0,
   *      if already alive, increment age
   *    else: if alive, make dead, set age to 0
   *      if dead, increment age.
   *    if age > max_age; append to erase list
   * */

  for(auto& element : global_variables.active){
    int x = element.first.first;
    int y = element.first.second;

    // format of read is (*read)[x][y], below line is a bounds check
    if(x < 1 || x > read->size() || y < 1 || y > (*read)[0].size()) continue;

    int sum = sum_neighbors(x, y);
    if(sum == 2){
      element.second++;  // incrementing age
    }
    else if(sum == 3){
      if((*read)[x][y]){
        element.second++; // incrementing age
      }
      else {
        element.second = 0;
        (*write)[x][y] = true;
      }
    }
    else {
      if((*read)[x][y]){
        element.second = 0;
        (*write)[x][y] = false;
      }
      else{
        element.second++;
      }
    }
    if(element.second == global_variables.max_age)
      erase_list.push_back({x, y});
  }
  for(auto victim : erase_list){
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
