#include <iostream>
#include "Renderer.hpp"

namespace kit2d {

  void Renderer::clear() {
    SDL_RenderClear(sdlRenderer);
  }

  void Renderer::drawTexture(Texture& texture) {
    SDL_RenderCopy(sdlRenderer, texture.internal_sdlTexture, nullptr, nullptr);
  }

  void Renderer::present() {
    SDL_RenderPresent(sdlRenderer);
  }
}
