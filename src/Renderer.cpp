#include <iostream>
#include "Renderer.hpp"
#include "../include/Err.hpp"

namespace kit2d {

  Renderer::Renderer() {}

  Renderer::~Renderer() {}

  void Renderer::setColor(uint8_t r, uint8_t g, uint8_t b, uint8_t a) {
    if (SDL_SetRenderDrawColor(sdlRenderer, r, g, b, a) != 0)
      throw Err {};
  }

  void Renderer::clear() {
    if (SDL_RenderClear(sdlRenderer) != 0)
      throw Err {};
  }

  void Renderer::drawTexture(Texture& texture, int x, int y) {
    auto& size = texture.getSize();
    texture.internal_destBounds.x = x;
    texture.internal_destBounds.y = y;
    texture.internal_destBounds.w = size.x;
    texture.internal_destBounds.h = size.y;
    std::cout << sdlRenderer << " " << std::flush;
    if (SDL_RenderCopy(sdlRenderer,
      texture.internal_sdlTexture,
      nullptr, nullptr) != 0) throw Err {};
  }

  void Renderer::present() {
    SDL_RenderPresent(sdlRenderer);
  }
}
