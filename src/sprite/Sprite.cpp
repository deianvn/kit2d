#include "../../include/kit2d/sprite/Sprite.hpp"

namespace kit2d {

  Sprite::Sprite() {}

  void Sprite::draw(Renderer& renderer) {
    renderer.draw(textureView,
      static_cast<int>(position.x),
      static_cast<int>(position.y));
  }

}
