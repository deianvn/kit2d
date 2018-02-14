#ifndef KIT2D_CORE_WINDOW_HPP
#define KIT2D_CORE_WINDOW_HPP

#include <cstdint>
#include <functional>

#include "SDL2.hpp"
#include "Context.hpp"
#include "Renderer.hpp"
#include "Texture.hpp"
#include "Flags.hpp"

namespace kit2d {

  class Window {
  public:
    using OnRenderCallback = std::function<void(Renderer& renderer)>;
    using OnUpdateCallback = std::function<void(float deltaTime)>;

    Window(const char* title,
      int x, int y,
      int width, int height,
      uint32_t flags);

    ~Window();

    void loop();
    Context& getContext();
    void onRender(OnRenderCallback onRenderCallback);
    void onUpdate(OnUpdateCallback OnUpdateCallback);
  private:
    SDL_Window* sdlWindow;
    SDL_Renderer* sdlRenderer;
    std::unique_ptr<Context> context;
    OnRenderCallback onRenderCallback;
    OnUpdateCallback onUpdateCallback;
  };

}

#endif
