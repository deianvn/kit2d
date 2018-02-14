#ifndef KIT2D_CORE_RENDERER_HPP
#define KIT2D_CORE_RENDERER_HPP

#include "SDL2.hpp"
#include "Context.hpp"
#include "Texture.hpp"

namespace kit2d {

  class Renderer {
  public:
    Renderer(Context& context);
    virtual ~Renderer();
    void setTarget();
    void setClipRect();
    void setColor(uint8_t r, uint8_t g, uint8_t b, uint8_t a);
    void clear();
    void draw(Texture& texture, int x, int y);
    void draw(TextureRegion textureRegion, int x, int y);
    void drawPoint();
    void drawLine();
    void drawRect();
    void fillRect();
    void present();
  private:
    Context* context { nullptr };
    SDL_Rect destRect {};
  };

}

#endif
