#ifndef KIT2D_WINDOW_HPP
#define KIT2D_WINDOW_HPP

#include <string>
#include <cstdint>
#include <functional>
#include "Flags.hpp"
#include "Renderer.hpp"
#include "Texture.hpp"

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
  };
}

#endif
