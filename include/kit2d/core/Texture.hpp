#ifndef KIT2D_CORE_TEXTURE_HPP
#define KIT2D_CORE_TEXTURE_HPP

#include "SDL2.hpp"
#include "Context.hpp"
#include "TextureRegion.hpp"
#include "Point.hpp"
#include "Rect.hpp"

namespace kit2d {

  class Texture {
  public:
    Texture(Context& context);
    ~Texture();
    void loadFromFile(const char* path);
    const Point& getSize() const;
    TextureRegion getRegion() const;
    TextureRegion getRegion(int x, int y, int width, int height) const;
    TextureRegion getRegion(Rect srcRect) const;

    SDL_Texture* sdlTexture { nullptr };
  private:
    Context* context { nullptr };
    Point size {};
  };

}

#endif
