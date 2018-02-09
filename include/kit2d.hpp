#ifndef KIT2D_KIT2D_HPP
#define KIT2D_KIT2D_HPP

#include <cstdint>
#include "../src/SDL2.hpp"

namespace kit2d {

  class Flags {
  public:
    const static Uint32
      INIT_TIMER = SDL_INIT_TIMER,
      INIT_AUDIO = SDL_INIT_AUDIO,
      INIT_VIDEO = SDL_INIT_VIDEO,
      INIT_JOYSTICK = SDL_INIT_JOYSTICK,
      INIT_HAPTIC = SDL_INIT_HAPTIC,
      INIT_GAMECONTROLLER = SDL_INIT_GAMECONTROLLER,
      INIT_EVENTS = SDL_INIT_EVENTS,
      INIT_EVERYTHING = SDL_INIT_EVERYTHING,
      INIT_NOPARACHUTE = SDL_INIT_NOPARACHUTE;

    const static int
      RESULT_ERROR = 0;
  };

  int init(Uint32 flags);

}

#endif
