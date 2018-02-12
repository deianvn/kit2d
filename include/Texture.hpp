#ifndef KIT2D_TEXTURE_HPP
#define KIT2D_TEXTURE_HPP

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
    TextureRegion getRegion(Rect srcRect);
  };

}

#endif
