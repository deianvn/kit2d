#ifndef KIT2D_FLAGS_HPP
#define KIT2D_FLAGS_HPP

#include <cstdint>
#include "../src/SDL2.hpp"

namespace kit2d {

  class Flags {
  public:
    const static Uint32
      INIT_TIMER,
      INIT_AUDIO,
      INIT_VIDEO,
      INIT_JOYSTICK,
      INIT_HAPTIC,
      INIT_GAMECONTROLLER,
      INIT_EVENTS,
      INIT_EVERYTHING,
      INIT_NOPARACHUTE,
      WINDOW_FULLSCREEN,
      WINDOW_FULLSCREEN_DESKTOP,
      WINDOW_OPENGL,
      WINDOW_SHOWN,
      WINDOW_HIDDEN,
      WINDOW_BORDERLESS,
      WINDOW_RESIZABLE,
      WINDOW_MINIMIZED,
      WINDOW_MAXIMIZED,
      WINDOW_INPUT_GRABBED,
      WINDOW_INPUT_FOCUS,
      WINDOW_MOUSE_FOCUS,
      WINDOW_FOREIGN,
      WINDOW_ALLOW_HIGHDPI,
      WINDOW_MOUSE_CAPTURE,
      WINDOW_ALWAYS_ON_TOP,
      WINDOW_SKIP_TASKBAR,
      WINDOW_UTILITY,
      WINDOW_TOOLTIP,
      WINDOW_POPUP_MENU;
    const static int
      RESULT_INIT_OK,
      WINDOWPOS_CENTERED,
      WINDOWPOS_UNDEFINED;
  };

}

#endif
