#ifndef KIT2D_WINDOW_HPP
#define KIT2D_WINDOW_HPP

#include <string>
#include "Flags.hpp"

namespace kit2d {

  class Window {
  public:

    Window(std::string title,
      int x, int y,
      int width, int height,
      Uint32 flags);

    ~Window();

    void loop();
  private:
    SDL_Window* sdlWindow;
  };

}

#endif
