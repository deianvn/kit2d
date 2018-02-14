#ifndef KIT2D_CORE_TEXTURE_HPP
#define KIT2D_CORE_TEXTURE_HPP

#include "SDL2.hpp"
#include "TextureRegion.hpp"
#include "Point.hpp"
#include "Rect.hpp"

namespace kit2d {

  class Texture {
  public:
    Texture(const Texture& texture) = delete;
    Texture(Texture&& texture);
    Texture(SDL_Texture* sdlTexture);
    ~Texture();
    const Point& getSize() const;
    const TextureRegion getRegion() const;
    const TextureRegion getRegion(int x, int y, int width, int height) const;
    inline const TextureRegion getRegion(Rect srcRect) const {
      return getRegion(srcRect.x, srcRect.y, srcRect.width, srcRect.height);
    }
    SDL_Texture* internal_sdlTexture;
  private:
    Point size {};
  };

}

#endif
