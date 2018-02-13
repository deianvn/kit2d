#ifndef KIT2D_SPRITE_HPP
#define KIT2D_SPRITE_HPP

#include "TextureRegion.hpp"
#include "Vec2f.hpp"

namespace kit2d {

  class Sprite {
  public:
    Sprite(TextureRegion textureRegion) :
      textureRegion(textureRegion) {}

    Sprite(TextureRegion textureRegion, float x, float y) :
      position(x, y), textureRegion(textureRegion) {}

    const TextureRegion& getTextureRegion() {
      return textureRegion;
    }

    Vec2f position;
  private:
    TextureRegion textureRegion;
  };

}

#endif
