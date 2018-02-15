#ifndef KIT2D_SPRITE_SPRITE_HPP
#define KIT2D_SPRITE_SPRITE_HPP

#include "../core/TextureView.hpp"
#include "../core/Renderer.hpp"
#include "Vec2.hpp"

namespace kit2d {

  class Sprite {
  public:
    Sprite();
    Sprite(TextureView textureView) :
      textureView(textureView) {}
    Sprite(TextureView textureView, float x, float y) :
      position(x, y), textureView(textureView) {}
    void setTextureView(TextureView textureView);
    void draw(Renderer& renderer);

    Vec2 position;
  private:
    TextureView textureView;
  };

}

#endif
