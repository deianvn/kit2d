#ifndef KIT2D_SPRITE_SPRITE_HPP
#define KIT2D_SPRITE_SPRITE_HPP

#include "../core/TextureRegion.hpp"
#include "../core/Renderer.hpp"
#include "Vec2.hpp"

namespace kit2d {

  class Sprite {
  public:
    Sprite() = default;
    Sprite(Sprite& sprite) = default;
    Sprite(Sprite&& sprite) = default;
    Sprite(TextureRegion textureRegion) :
      textureRegion(textureRegion) {}
    Sprite(TextureRegion textureRegion, float x, float y) :
      position(x, y), textureRegion(textureRegion) {}
    void setTextureRegion(TextureRegion textureRegion);
    void draw(Renderer& renderer);

    Vec2 position;
  private:
    TextureRegion textureRegion;
  };

}

#endif
