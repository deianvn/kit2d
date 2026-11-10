#include "../../include/kit2d/core/SDL2.hpp"
#include "../../include/kit2d/core/Texture.hpp"
#include "../../include/kit2d/core/Err.hpp"
#include "Context.hpp"

namespace kit2d {

    Texture::Texture() {
    }

    Texture::Texture(Texture&& texture) {
        sdlTexture = texture.sdlTexture;
        size = texture.size;
        texture.sdlTexture = nullptr;
    }

    Texture& Texture::operator=(Texture&& other) {
        if (this != &other) {
            SDL_DestroyTexture(sdlTexture);
            sdlTexture = other.sdlTexture;
            size = other.size;
            other.sdlTexture = nullptr;
        }
        return *this;
    }

    Texture::~Texture() {
        SDL_DestroyTexture(sdlTexture);
    }

    void Texture::loadFromFile(const char* path) {
        sdlTexture = IMG_LoadTexture(Context::handle, path);
        if (sdlTexture == nullptr) {
            throw Err {};
        }
        SDL_QueryTexture(sdlTexture, nullptr, nullptr, &size.x, &size.y);
    }

    const Point& Texture::getSize() const {
        return size;
    }

    Texture Texture::createSolid(
        int w,
        int h,
        uint8_t r,
        uint8_t g,
        uint8_t b,
        uint8_t a
    ) {
        Texture t;
        t.sdlTexture = SDL_CreateTexture(
            Context::handle, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, w, h
        );
        if (!t.sdlTexture)
            throw Err {};
        SDL_SetRenderTarget(Context::handle, t.sdlTexture);
        SDL_SetRenderDrawColor(Context::handle, r, g, b, a);
        SDL_RenderClear(Context::handle);
        SDL_SetRenderTarget(Context::handle, nullptr);
        t.size = Point { w, h };
        return t;
    }

}
