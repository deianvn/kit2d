#include "../include/kit2d.hpp"

namespace kit2d {

  int init(Uint32 flags) {
    return SDL_Init(flags);
  }

}
