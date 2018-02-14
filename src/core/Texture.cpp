#include "../../include/kit2d/core/SDL2.hpp"
#include "../../include/kit2d/core/Texture.hpp"
#include "../../include/kit2d/core/Err.hpp"

#include <iostream>

namespace kit2d {

  Texture::Texture(Context& context) : context(&context) {}

  Texture::~Texture() {
    SDL_DestroyTexture(sdlTexture);
  }

  void Texture::loadFromFile(const char* path) {
    sdlTexture = IMG_LoadTexture(context->handle, path);
    if (sdlTexture == nullptr) {
      throw Err {};
    }
    SDL_QueryTexture(sdlTexture, nullptr, nullptr, &size.x, &size.y);
  }

  const Point& Texture::getSize() const {
    return size;
  }

  TextureRegion Texture::getRegion() const {
    return TextureRegion {
      sdlTexture,
      SDL_Rect { 0, 0, size.x, size.y }
    };
  }

  TextureRegion Texture::getRegion(Rect srcRect) const {
    return getRegion(srcRect.x, srcRect.y, srcRect.width, srcRect.height);
  }

  TextureRegion Texture::getRegion(int x, int y, int width, int height) const {
    return TextureRegion {
      sdlTexture,
      SDL_Rect { x, y, width, height }
    };
  }

}
