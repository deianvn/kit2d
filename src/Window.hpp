#ifndef KIT2D_WINDOW_HPP
#define KIT2D_WINDOW_HPP

#include <string>
#include <cstdint>
#include "SDL2.hpp"
#include "../include/Flags.hpp"

namespace kit2d {

  class Window {
  public:

    Window(std::string title,
      int x, int y,
      int width, int height,
      uint32_t flags);

    ~Window();

    void loop();
  private:
    SDL_Window* sdlWindow;
  };

}

#endif
