#include "../../include/kit2d/sprite/Sprite.hpp"

namespace kit2d {

  void Sprite::draw(Renderer& renderer) {
    renderer.draw(textureRegion,
      static_cast<int>(position.x),
      static_cast<int>(position.y));
  }

}
