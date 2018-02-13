#ifndef KIT2D_RENDERER_HPP
#define KIT2D_RENDERER_HPP

#include "SDL2.hpp"
#include "Texture.hpp"

#include <iostream>

namespace kit2d {

  class Renderer {
  public:
    Renderer(SDL_Renderer* sdlRenderer);
    ~Renderer();
    void setTarget();
    void setClipRect();
    void setColor(uint8_t r, uint8_t g, uint8_t b, uint8_t a);
    void clear();
    void drawTexture(Texture& texture, int x, int y);
    void drawRegion();
    void drawPoint();
    void drawLine();
    void drawRect();
    void fillRect();
    void present();
  private:
    SDL_Renderer* sdlRenderer;
    SDL_Rect destRect {};
  };

}

#endif
