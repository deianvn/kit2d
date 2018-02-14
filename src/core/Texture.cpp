#include "../../include/kit2d/core/SDL2.hpp"
#include "../../include/kit2d/core/Texture.hpp"

#include <iostream>

namespace kit2d {

  Texture::Texture(Texture&& texture) {
    internal_sdlTexture = texture.internal_sdlTexture;
    SDL_QueryTexture(internal_sdlTexture, nullptr, nullptr, &size.x, &size.y);
  }

  Texture::Texture(SDL_Texture* sdlTexture) {
    std::cout << "Creating texture" << std::endl;
    internal_sdlTexture = sdlTexture;
    SDL_QueryTexture(internal_sdlTexture, nullptr, nullptr, &size.x, &size.y);
  }
  Texture::~Texture() {
    std::cout << "Destroying texture..." << std::endl;
    SDL_DestroyTexture(internal_sdlTexture);
  }
  const Point& Texture::getSize() const {
    return size;
  }
  const TextureRegion Texture::getRegion() const {
    return TextureRegion {
      internal_sdlTexture,
      SDL_Rect { 0, 0, size.x, size.y }
    };
  }
  const TextureRegion Texture::getRegion(int x, int y, int width, int height) const {
    return TextureRegion {
      internal_sdlTexture,
      SDL_Rect { x, y, width, height }
    };
  }

}
