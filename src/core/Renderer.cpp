#include <iostream>
#include <string>
#include "../../include/kit2d/core/Renderer.hpp"
#include "../../include/kit2d/core/Err.hpp"
#include "Context.hpp"

namespace kit2d {

    Renderer::Renderer() {
    }

    Renderer::~Renderer() {
    }

    void Renderer::setTarget(Texture* texture) {
        SDL_Texture* target = texture ? texture->sdlTexture : nullptr;
        if (SDL_SetRenderTarget(Context::handle, target) != 0)
            throw Err {};
    }

    void Renderer::setClipRect(const Rect* rect) {
        if (rect) {
            SDL_Rect sdlRect { rect->x, rect->y, rect->width, rect->height };
            SDL_RenderSetClipRect(Context::handle, &sdlRect);
        } else {
            SDL_RenderSetClipRect(Context::handle, nullptr);
        }
    }

    void Renderer::setColor(
        uint8_t r,
        uint8_t g,
        uint8_t b,
        uint8_t a
    ) {
        if (SDL_SetRenderDrawColor(Context::handle, r, g, b, a) != 0)
            throw Err {};
    }

    void Renderer::clear() {
        if (SDL_RenderClear(Context::handle) != 0)
            throw Err {};
    }

    void Renderer::draw(
        Texture& texture,
        int x,
        int y
    ) {
        if (!texture.sdlTexture)
            return;
        auto& size = texture.getSize();
        destRect.x = x;
        destRect.y = y;
        destRect.w = size.x;
        destRect.h = size.y;
        if (SDL_RenderCopy(Context::handle, texture.sdlTexture, nullptr, &destRect) != 0)
            throw Err {};
    }

    void Renderer::draw(
        TextureView textureView,
        int x,
        int y
    ) {
        if (!textureView.sdlTexture)
            return;
        destRect.x = x;
        destRect.y = y;
        destRect.w = textureView.sdlTextureRect.w;
        destRect.h = textureView.sdlTextureRect.h;
        if (SDL_RenderCopy(
                Context::handle, textureView.sdlTexture, &textureView.sdlTextureRect, &destRect
            ) != 0)
            throw Err {};
    }

    void Renderer::drawGeometry(
        SDL_Texture* texture,
        const SDL_Vertex* vertices,
        int vertexCount,
        const int* indices,
        int indexCount,
        SDL_BlendMode blendMode
    ) {

        SDL_BlendMode previousBlendMode;
        if (SDL_GetTextureBlendMode(texture, &previousBlendMode) != 0)
            throw Err {};
        if (SDL_SetTextureBlendMode(texture, blendMode) != 0)
            throw Err {};
        if (SDL_RenderGeometry(
                Context::handle, texture, vertices, vertexCount, indices, indexCount
            ) != 0) {
            const Err error {};
            if (SDL_SetTextureBlendMode(texture, previousBlendMode) != 0) {
                throw std::runtime_error { std::string(error.what()) +
                    "; restoring texture blending failed: " + SDL_GetError() };
            }
            throw error;
        }
        if (SDL_SetTextureBlendMode(texture, previousBlendMode) != 0)
            throw Err {};
    }

    void Renderer::drawPoint(int x, int y) {
        if (SDL_RenderDrawPoint(Context::handle, x, y) != 0)
            throw Err {};
    }

    void Renderer::drawLine(
        int x1,
        int y1,
        int x2,
        int y2
    ) {
        if (SDL_RenderDrawLine(Context::handle, x1, y1, x2, y2) != 0)
            throw Err {};
    }

    void Renderer::drawRect(const Rect& rect) {
        SDL_Rect sdlRect { rect.x, rect.y, rect.width, rect.height };
        if (SDL_RenderDrawRect(Context::handle, &sdlRect) != 0)
            throw Err {};
    }

    void Renderer::fillRect(const Rect& rect) {
        SDL_Rect sdlRect { rect.x, rect.y, rect.width, rect.height };
        if (SDL_RenderFillRect(Context::handle, &sdlRect) != 0)
            throw Err {};
    }

    void Renderer::present() {
        SDL_RenderPresent(Context::handle);
    }
}
