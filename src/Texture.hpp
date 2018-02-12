#ifndef KIT2D_TEXTURE_HPP
#define KIT2D_TEXTURE_HPP

#include "SDL2.hpp"
#include "TextureRegion.hpp"
#include "../include/Rect.hpp"

namespace kit2d {

  class Texture {
  public:
    TextureRegion getRegion(int x, int y, int width, int height);

    inline TextureRegion getRegion(Rect srcRect) {
      return getRegion(srcRect.x, srcRect.y, srcRect.width, srcRect.height);
    }

    SDL_Texture* internal_sdlTexture;
  };

}

#endif
