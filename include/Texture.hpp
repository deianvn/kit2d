#ifndef KIT2D_TEXTURE_HPP
#define KIT2D_TEXTURE_HPP

#include "SDL2.hpp"
#include "TextureRegion.hpp"
#include "Point.hpp"
#include "Rect.hpp"

namespace kit2d {

  class Texture {
  public:
    Texture();
    ~Texture();
    Point& getSize();
    TextureRegion getRegion(int x, int y, int width, int height);
    inline TextureRegion getRegion(Rect srcRect) {
      return getRegion(srcRect.x, srcRect.y, srcRect.width, srcRect.height);
    }
    void internal_setSdlTexture(SDL_Texture* sdlTexture);
    SDL_Texture* internal_sdlTexture;
  private:
    Point size {};
  };

}

#endif
