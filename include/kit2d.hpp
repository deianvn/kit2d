#ifndef KIT2D_KIT2D_HPP
#define KIT2D_KIT2D_HPP

#include "../src/SDL2.hpp"
#include "Flags.hpp"
#include "Window.hpp"

namespace kit2d {

  int init(Uint32 flags);

  const char* getInitError();

  void quit();

  void delay(int milliseconds);
}

#endif
