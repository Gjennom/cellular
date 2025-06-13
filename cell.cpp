#include "cell.h"

GlobalVariables global_variables;

int main(int argc, char* argv[]){
  if(argc != 4)
    InitWindow(1000,600,"Cellular Automata");
  else{
    global_variables.gui_height = std::stoi(argv[2]);
    global_variables.gui_width = std::stoi(argv[1]);
    global_variables.density = std::stof(argv[3]);
    InitWindow(std::stoi(argv[1]),std::stoi(argv[2]),"Cellular Automata");
  }
  global_variables.ping = make_random(global_variables.gui_width, global_variables.gui_height, global_variables.density);
  global_variables.pong = global_variables.ping;
  SetTargetFPS(global_variables.FPS);
  initial_pass();
  while(!WindowShouldClose()){
    //get_input();
    update();
    display();
  }
  CloseWindow();
}
