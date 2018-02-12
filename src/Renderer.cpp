#include <iostream>
#include "Renderer.hpp"

namespace kit2d {

  void Renderer::clear() {
    SDL_RenderClear(sdlRenderer);
  }

  void Renderer::drawTexture(Texture& texture, int x, int y) {
    texture.internal_destBounds.x = x;
    texture.internal_destBounds.y = y;
    SDL_RenderCopy(sdlRenderer, texture.internal_sdlTexture, nullptr, &texture.internal_destBounds);
  }

  void Renderer::present() {
    SDL_RenderPresent(sdlRenderer);
  }
}
