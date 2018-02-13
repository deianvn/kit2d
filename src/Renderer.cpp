#include <iostream>
#include "../include/Renderer.hpp"
#include "../include/Err.hpp"

namespace kit2d {

  Renderer::Renderer(SDL_Renderer* sdlRenderer) {
    this->sdlRenderer = sdlRenderer;
  }

  Renderer::~Renderer() {}

  void Renderer::setColor(uint8_t r, uint8_t g, uint8_t b, uint8_t a) {
    if (SDL_SetRenderDrawColor(sdlRenderer, r, g, b, a) != 0)
      throw Err {};
  }

  void Renderer::clear() {
    if (SDL_RenderClear(sdlRenderer) != 0)
      throw Err {};
  }

  void Renderer::draw(Texture& texture, int x, int y) {
    auto& size = texture.getSize();
    destRect.x = x;
    destRect.y = y;
    destRect.w = size.x;
    destRect.h = size.y;
    if (SDL_RenderCopy(sdlRenderer, texture.internal_sdlTexture,
      nullptr, &destRect) != 0) throw Err {};
  }

  void Renderer::draw(TextureRegion textureRegion, int x, int y) {
    destRect.x = x;
    destRect.y = y;
    destRect.w = textureRegion.internal_sdl_srcRect.w;
    destRect.h = textureRegion.internal_sdl_srcRect.h;
    if (SDL_RenderCopy(sdlRenderer, textureRegion.internal_sdl_texture,
      &textureRegion.internal_sdl_srcRect, &destRect) != 0) throw Err {};
  }

  void Renderer::present() {
    SDL_RenderPresent(sdlRenderer);
  }
}
