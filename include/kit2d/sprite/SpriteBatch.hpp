#ifndef KIT2D_SPRITE_SPRITE_BATCH_HPP
#define KIT2D_SPRITE_SPRITE_BATCH_HPP

#include <vector>

#include "Sprite.hpp"
#include "../core/Renderer.hpp"

namespace kit2d {

  class SpriteBatch {
  public:
    void add(Sprite& sprite);
    void process(Renderer& renderer);
  private:
    std::vector<Sprite*> sprites {};
  };

}

#endif
