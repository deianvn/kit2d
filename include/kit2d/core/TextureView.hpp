#ifndef KIT2D_CORE_TEXTURE_VIEW_HPP
#define KIT2D_CORE_TEXTURE_VIEW_HPP

#include "SDL2.hpp"
#include "Texture.hpp"

namespace kit2d {

  class TextureView {
  public:
    TextureView();
    TextureView(const Texture& texture);
    TextureView(const Texture& texture, const Rect textureRect);
    void setTexture(const Texture& texture);
    void setTexture(const Texture& texture, const Rect textureRect);
    void setTextureRect(const Rect& textureRect);

    SDL_Texture* sdlTexture { nullptr };
    SDL_Rect sdlTextureRect {};
  };

}

#endif
