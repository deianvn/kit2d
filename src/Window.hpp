#ifndef KIT2D_WINDOW_HPP
#define KIT2D_WINDOW_HPP

#include <cstdint>
#include <functional>
#include "SDL2.hpp"
#include "Renderer.hpp"
#include "Texture.hpp"
#include "../include/Flags.hpp"

namespace kit2d {

  class Window {
  public:
    using OnRenderCallback = std::function<void(Renderer& renderer)>;

    Window(const char* title,
      int x, int y,
      int width, int height,
      uint32_t flags);

    ~Window();

    void loop();
    Renderer renderer();
    void onRender(OnRenderCallback onRenderCallback);
    Texture loadTexture(const char* path);
  private:
    SDL_Window* sdlWindow;
    SDL_Renderer* sdlRenderer;
    OnRenderCallback onRenderCallback;
  };

}

#endif
