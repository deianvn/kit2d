#include <iostream>
#include "../../include/kit2d/core/Renderer.hpp"
#include "../../include/kit2d/core/Err.hpp"
#include "Context.hpp"

namespace kit2d {

  Renderer::Renderer() {}

  Renderer::~Renderer() {}

  void Renderer::setColor(uint8_t r, uint8_t g, uint8_t b, uint8_t a) {
    if (SDL_SetRenderDrawColor(Context::handle, r, g, b, a) != 0)
      throw Err {};
  }

  void Renderer::clear() {
    if (SDL_RenderClear(Context::handle) != 0)
      throw Err {};
  }

  void Renderer::draw(Texture& texture, int x, int y) {
    auto& size = texture.getSize();
    destRect.x = x;
    destRect.y = y;
    destRect.w = size.x;
    destRect.h = size.y;
    if (SDL_RenderCopy(Context::handle, texture.sdlTexture,
      nullptr, &destRect) != 0) throw Err {};
  }

  void Renderer::draw(TextureView textureView, int x, int y) {
    destRect.x = x;
    destRect.y = y;
    destRect.w = textureView.sdlTextureRect.w;
    destRect.h = textureView.sdlTextureRect.h;
    if (SDL_RenderCopy(Context::handle, textureView.sdlTexture,
      &textureView.sdlTextureRect, &destRect) != 0) throw Err {};
  }

  void Renderer::present() {
    SDL_RenderPresent(Context::handle);
  }
}
