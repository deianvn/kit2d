#ifndef KIT2D_SPRITE_HPP
#define KIT2D_SPRITE_HPP

#include "TextureRegion.hpp"
#include "Vec2.hpp"

namespace kit2d {

  class Sprite {
  public:
    Sprite(TextureRegion textureRegion) :
      textureRegion(textureRegion) {}

    Sprite(TextureRegion textureRegion, float x, float y) :
      position(x, y), textureRegion(textureRegion) {}

    void draw(Renderer& renderer) {
      renderer.draw(textureRegion,
        static_cast<int>(position.x),
        static_cast<int>(position.y));
    }

    Vec2 position;
  private:
    TextureRegion textureRegion;
  };

}

#endif
