#include <iostream>
#include "../../include/kit2d/core/Renderer.hpp"
#include "../../include/kit2d/core/Err.hpp"

namespace kit2d {

  Renderer::Renderer(Context& context) : context(&context) {}

  Renderer::~Renderer() {}

  void Renderer::setColor(uint8_t r, uint8_t g, uint8_t b, uint8_t a) {
    if (SDL_SetRenderDrawColor(context->handle, r, g, b, a) != 0)
      throw Err {};
  }

  void Renderer::clear() {
    if (SDL_RenderClear(context->handle) != 0)
      throw Err {};
  }

  void Renderer::draw(Texture& texture, int x, int y) {
    auto& size = texture.getSize();
    destRect.x = x;
    destRect.y = y;
    destRect.w = size.x;
    destRect.h = size.y;
    if (SDL_RenderCopy(context->handle, texture.sdlTexture,
      nullptr, &destRect) != 0) throw Err {};
  }

  void Renderer::draw(TextureRegion textureRegion, int x, int y) {
    destRect.x = x;
    destRect.y = y;
    destRect.w = textureRegion.sdlSrcRect.w;
    destRect.h = textureRegion.sdlSrcRect.h;
    if (SDL_RenderCopy(context->handle, textureRegion.sdlTexture,
      &textureRegion.sdlSrcRect, &destRect) != 0) throw Err {};
  }

  void Renderer::present() {
    SDL_RenderPresent(context->handle);
  }
}
