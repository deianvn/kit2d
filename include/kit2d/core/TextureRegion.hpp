#ifndef KIT2D_CORE_TEXTURE_REGION_HPP
#define KIT2D_CORE_TEXTURE_REGION_HPP

#include "SDL2.hpp"

namespace kit2d {

  class TextureRegion {
  public:
    TextureRegion(SDL_Texture* texture, SDL_Rect srcRect) :
      internal_sdl_texture(texture),
      internal_sdl_srcRect(srcRect) {}

    SDL_Texture* internal_sdl_texture;
    SDL_Rect internal_sdl_srcRect;
  };

}

#endif
