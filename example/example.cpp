#include <iostream>
#include <string>
#include "../include/kit2d.hpp"

using namespace kit2d;

int main() {
  if (init(Flags::INIT_VIDEO) == Flags::RESULT_INIT_OK) {
    auto window = std::make_unique<Window>(std::string { "My window" },
      Flags::WINDOWPOS_UNDEFINED, Flags::WINDOWPOS_UNDEFINED,
      640, 480,
      Flags::WINDOW_OPENGL);
    window->loop();
  }
  quit();
  return 0;
}
