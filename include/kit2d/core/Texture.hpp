#ifndef KIT2D_CORE_TEXTURE_HPP
#define KIT2D_CORE_TEXTURE_HPP

#include "SDL2.hpp"
#include "Point.hpp"
#include "Rect.hpp"

namespace kit2d {

  class Texture {
  public:
    Texture();
    Texture(const Texture& texture) = default;
    Texture(Texture&& texture);
    ~Texture();
    void loadFromFile(const char* path);
    const Point& getSize() const;

    SDL_Texture* sdlTexture { nullptr };
  private:
    Point size {};
  };

}

#endif
