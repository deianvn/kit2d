#ifndef KIT2D_WINDOW_HPP
#define KIT2D_WINDOW_HPP

#include <string>

namespace kit2d {

  class Window {
  public:
    Window(std::string title,
      int x, int y,
      int width, int height,
      Uint32 flags) {

      
    }
  private:
    SDL_Window* sdlWindow;
  };

}

#endif
