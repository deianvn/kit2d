#include <iostream>
#include "../include/kit2d.hpp"

namespace kit2d {

  int init(Uint32 flags) {
    return SDL_Init(flags);
  }

  const char* getInitError() {
    return SDL_GetError();
  }

  void quit() {
    SDL_Quit();
  }

  void delay(int milliseconds) {
    SDL_Delay(milliseconds);
  }
}
