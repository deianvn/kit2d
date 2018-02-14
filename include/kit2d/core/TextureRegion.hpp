#ifndef KIT2D_CORE_TEXTURE_REGION_HPP
#define KIT2D_CORE_TEXTURE_REGION_HPP

#include "SDL2.hpp"

namespace kit2d {

  class TextureRegion {
  public:
    TextureRegion() = default;
    TextureRegion(const TextureRegion& textureRegion) = default;
    TextureRegion(TextureRegion&& textureRegion) = default;
    TextureRegion(SDL_Texture* texture, SDL_Rect srcRect) :
      sdlTexture(texture),
      sdlSrcRect(srcRect) {}

    SDL_Texture* sdlTexture { nullptr };
    SDL_Rect sdlSrcRect {};
  };

}

#endif
