#ifndef KIT2D_RENDERER_HPP
#define KIT2D_RENDERER_HPP

#include "Texture.hpp"

namespace kit2d {

  class Renderer {
  public:
    void setTarget();
    void setClipRect();
    void setColor();
    void clear();
    void drawTexture(Texture& texture, int x, int y);
    void drawRegion();
    void drawPoint();
    void drawLine();
    void drawRect();
    void fillRect();
    void present();
  };

}

#endif
