#include "../include/SDL2.hpp"
#include "../include/Texture.hpp"

#include <iostream>

namespace kit2d {

  Texture::Texture(SDL_Texture* sdlTexture) {
    internal_sdlTexture = sdlTexture;
    SDL_QueryTexture(internal_sdlTexture, nullptr, nullptr, &size.x, &size.y);
  }
  Texture::~Texture() {
    SDL_DestroyTexture(internal_sdlTexture);
  }
  const Point& Texture::getSize() {
    return size;
  }
  const TextureRegion Texture::getRegion() {
    return TextureRegion {
      internal_sdlTexture,
      SDL_Rect { 0, 0, size.x, size.y }
    };
  }
  const TextureRegion Texture::getRegion(int x, int y, int width, int height) {
    return TextureRegion {
      internal_sdlTexture,
      SDL_Rect { x, y, width, height }
    };
  }

}
