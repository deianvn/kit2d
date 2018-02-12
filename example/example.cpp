#include <iostream>
#include <string>
#include "../include/kit2d.hpp"

using namespace kit2d;

int main() {
  Kit kit { Flags::INIT_VIDEO };
  {
    Window window(std::string { "My window" },
      Flags::WINDOWPOS_UNDEFINED, Flags::WINDOWPOS_UNDEFINED,
      640, 480,
      Flags::WINDOW_OPENGL);
    window.loop();
  }
  Kit::DEFAULT->quit();
  return 0;
}
