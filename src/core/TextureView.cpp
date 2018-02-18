#include "../../include/kit2d/core/TextureView.hpp"

namespace kit2d {

  TextureView::TextureView() {}

  TextureView::TextureView(const Texture& texture) :
    sdlTexture(texture.sdlTexture) {

    Point size = texture.getSize();
    sdlTextureRect.x = 0;
    sdlTextureRect.y = 0;
    sdlTextureRect.w = size.x;
    sdlTextureRect.h = size.y;
  }

  TextureView::TextureView(const Texture& texture, Rect textureRect) :
    sdlTexture(texture.sdlTexture) {

    sdlTextureRect.x = textureRect.x;
    sdlTextureRect.y = textureRect.y;
    sdlTextureRect.w = textureRect.width;
    sdlTextureRect.h = textureRect.height;
  }

  void TextureView::setTexture(const Texture& texture) {
    sdlTexture = texture.sdlTexture;
    Point size = texture.getSize();
    sdlTextureRect.x = 0;
    sdlTextureRect.y = 0;
    sdlTextureRect.w = size.x;
    sdlTextureRect.h = size.y;
  }

  void TextureView::setTexture(const Texture& texture, const Rect textureRect) {
    sdlTexture = texture.sdlTexture;
    sdlTextureRect.x = textureRect.x;
    sdlTextureRect.y = textureRect.y;
    sdlTextureRect.w = textureRect.width;
    sdlTextureRect.h = textureRect.height;
  }

  void TextureView::setTextureRect(const Rect& textureRect) {
    sdlTextureRect.x = textureRect.x;
    sdlTextureRect.y = textureRect.y;
    sdlTextureRect.w = textureRect.width;
    sdlTextureRect.h = textureRect.height;
  }

}
