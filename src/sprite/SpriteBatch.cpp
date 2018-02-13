#include "../../include/kit2d/sprite/SpriteBatch.hpp"
#include "../../include/kit2d/core/Renderer.hpp"

namespace kit2d {

  void SpriteBatch::add(Sprite& sprite) {
    sprites.push_back(&sprite);
  }

  void SpriteBatch::process(Renderer& renderer) {
    for (auto sprite : sprites) {
      sprite->draw(renderer);
    }
    renderer.present();
  }

}
