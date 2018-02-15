#include "../../include/kit2d/core/SDL2.hpp"
#include "../../include/kit2d/core/Texture.hpp"
#include "../../include/kit2d/core/Err.hpp"
#include "Context.hpp"

#include <iostream>

namespace kit2d {

  Texture::Texture() {}

  Texture::Texture(Texture&& texture) {
    sdlTexture = texture.sdlTexture;
    size = texture.size;
    texture.sdlTexture = nullptr;
  }

  Texture::~Texture() {
    std::cout << sdlTexture << std::endl;
    SDL_DestroyTexture(sdlTexture);
  }

  void Texture::loadFromFile(const char* path) {
    sdlTexture = IMG_LoadTexture(Context::handle, path);
    if (sdlTexture == nullptr) {
      throw Err {};
    }
    SDL_QueryTexture(sdlTexture, nullptr, nullptr, &size.x, &size.y);
  }

  const Point& Texture::getSize() const {
    return size;
  }

}
