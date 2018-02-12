#include "SDL2.hpp"
#include "Texture.hpp"

#include <iostream>

namespace kit2d {

  Texture::Texture() {}
  Texture::~Texture() {
    SDL_DestroyTexture(internal_sdlTexture);
  }
  Point& Texture::getSize() {
    return size;
  }
  TextureRegion Texture::getRegion(int x, int y, int width, int height) {
    return TextureRegion {};
  }
  void Texture::internal_setSdlTexture(SDL_Texture* sdlTexture) {
    internal_sdlTexture = sdlTexture;
    SDL_QueryTexture(internal_sdlTexture, nullptr, nullptr, &size.x, &size.y);
    internal_destBounds.w = size.x;
    internal_destBounds.h = size.y;
  }
}
