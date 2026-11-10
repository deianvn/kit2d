#include "../../include/kit2d/sprite/SpriteBatch.hpp"
#include "../../include/kit2d/core/Renderer.hpp"
#include "Geometry.hpp"

#include <algorithm>
#include <stdexcept>

namespace kit2d {

    SpriteBatch::SpriteBatch(std::size_t size) : capacity(size) {
        if (size < 1 || size > 8191) {
            throw std::invalid_argument { "Sprite batch capacity must be between 1 and 8191" };
        }
        vertices.reserve(size * 4);
        indices.reserve(size * 6);
        for (std::size_t i = 0; i < size; ++i) {
            const int base = static_cast<int>(i * 4);
            for (int index : { 0, 1, 2, 2, 3, 0 }) {
                indices.push_back(base + index);
            }
        }
    }

    void SpriteBatch::begin() {
        begin(defaultRenderer);
    }

    void SpriteBatch::begin(Renderer& renderer) {
        if (isDrawing())
            throw std::logic_error { "Sprite batch has already begun" };
        this->renderer = &renderer;
        renderCalls = 0;
        lastTexture = nullptr;
    }

    void SpriteBatch::end() {
        requireDrawing();
        flush();
        renderer = nullptr;
        lastTexture = nullptr;
    }

    void SpriteBatch::flush() {
        requireDrawing();
        if (vertices.empty())
            return;
        const auto spriteCount = vertices.size() / 4;
        renderer->drawGeometry(
            lastTexture, vertices.data(), static_cast<int>(vertices.size()), indices.data(),
            static_cast<int>(spriteCount * 6),
            blendingEnabled ? SDL_BLENDMODE_BLEND : SDL_BLENDMODE_NONE
        );
        ++renderCalls;
        maxSpritesInBatch = std::max(maxSpritesInBatch, static_cast<int>(spriteCount));
        vertices.clear();
    }

    bool SpriteBatch::isDrawing() const {
        return renderer != nullptr;
    }

    void SpriteBatch::requireDrawing() const {
        if (!isDrawing())
            throw std::logic_error { "Sprite batch drawing requires begin/end" };
    }

    void SpriteBatch::setColor(
        float r,
        float g,
        float b,
        float a
    ) {
        color = detail::makeColor(r, g, b, a);
    }

    const SDL_Color& SpriteBatch::getColor() const {
        return color;
    }

    void SpriteBatch::enableBlending() {
        if (blendingEnabled)
            return;
        if (isDrawing())
            flush();
        blendingEnabled = true;
    }

    void SpriteBatch::disableBlending() {
        if (!blendingEnabled)
            return;
        if (isDrawing())
            flush();
        blendingEnabled = false;
    }

    bool SpriteBatch::isBlendingEnabled() const {
        return blendingEnabled;
    }

    void SpriteBatch::append(
        SDL_Texture* texture,
        const SDL_Vertex* source,
        std::size_t count
    ) {
        for (std::size_t offset = 0; offset < count; offset += 4) {
            if (texture != lastTexture || vertices.size() == capacity * 4)
                flush();
            lastTexture = texture;
            vertices.insert(vertices.end(), source + offset, source + offset + 4);
        }
    }

    void SpriteBatch::draw(
        const Texture& texture,
        float x,
        float y
    ) {
        draw(TextureView { texture }, x, y);
    }

    void SpriteBatch::draw(
        const Texture& texture,
        float x,
        float y,
        float width,
        float height
    ) {
        draw(TextureView { texture }, x, y, width, height);
    }

    void SpriteBatch::draw(
        const Texture& texture,
        float x,
        float y,
        int srcX,
        int srcY,
        int srcWidth,
        int srcHeight
    ) {

        draw(TextureView { texture, Rect { srcX, srcY, srcWidth, srcHeight } }, x, y);
    }

    void SpriteBatch::draw(
        const Texture& texture,
        float x,
        float y,
        float width,
        float height,
        int srcX,
        int srcY,
        int srcWidth,
        int srcHeight,
        bool flipX,
        bool flipY
    ) {

        draw(
            texture, x, y, 0.0f, 0.0f, width, height, 1.0f, 1.0f, 0.0f, srcX, srcY, srcWidth,
            srcHeight, flipX, flipY
        );
    }

    void SpriteBatch::draw(
        const Texture& texture,
        float x,
        float y,
        float originX,
        float originY,
        float width,
        float height,
        float scaleX,
        float scaleY,
        float rotation,
        int srcX,
        int srcY,
        int srcWidth,
        int srcHeight,
        bool flipX,
        bool flipY
    ) {

        requireDrawing();
        if (!texture.sdlTexture)
            return;
        const auto quad = detail::makeQuad(
            TextureView { texture, Rect { srcX, srcY, srcWidth, srcHeight } }, x, y, originX,
            originY, width, height, scaleX, scaleY, rotation, color, flipX, flipY
        );
        append(texture.sdlTexture, quad.data(), quad.size());
    }

    void SpriteBatch::draw(
        const Texture& texture,
        float x,
        float y,
        float width,
        float height,
        float u,
        float v,
        float u2,
        float v2
    ) {

        requireDrawing();
        if (!texture.sdlTexture)
            return;
        const auto quad = detail::makeQuad(
            x, y, 0.0f, 0.0f, width, height, 1.0f, 1.0f, 0.0f, u, v, u2, v2, color
        );
        append(texture.sdlTexture, quad.data(), quad.size());
    }

    void SpriteBatch::draw(
        const Texture& texture,
        const std::vector<SDL_Vertex>& source,
        std::size_t offset,
        std::size_t count
    ) {

        requireDrawing();
        if (offset > source.size() || count > source.size() - offset || count % 4 != 0) {
            throw std::invalid_argument {
                "Raw sprite vertices must contain complete quads within the supplied range"
            };
        }
        for (std::size_t i = offset; i < offset + count; ++i) {
            const auto& vertex = source[i];
            detail::requireFinite(
                { vertex.position.x, vertex.position.y, vertex.tex_coord.x, vertex.tex_coord.y }
            );
            if (vertex.tex_coord.x < 0.0f || vertex.tex_coord.x > 1.0f ||
                vertex.tex_coord.y < 0.0f || vertex.tex_coord.y > 1.0f) {
                throw std::invalid_argument { "Texture coordinates must be between zero and one" };
            }
        }
        if (!texture.sdlTexture || count == 0)
            return;
        append(texture.sdlTexture, source.data() + offset, count);
    }

    void SpriteBatch::draw(
        const TextureView& region,
        float x,
        float y
    ) {
        draw(
            region, x, y, static_cast<float>(region.sdlTextureRect.w),
            static_cast<float>(region.sdlTextureRect.h)
        );
    }

    void SpriteBatch::draw(
        const TextureView& region,
        float x,
        float y,
        float width,
        float height
    ) {
        draw(region, x, y, 0.0f, 0.0f, width, height, 1.0f, 1.0f, 0.0f);
    }

    void SpriteBatch::draw(
        const TextureView& region,
        float x,
        float y,
        float originX,
        float originY,
        float width,
        float height,
        float scaleX,
        float scaleY,
        float rotation
    ) {

        requireDrawing();
        if (!region.sdlTexture)
            return;
        const auto quad = detail::makeQuad(
            region, x, y, originX, originY, width, height, scaleX, scaleY, rotation, color
        );
        append(region.sdlTexture, quad.data(), quad.size());
    }

    void SpriteBatch::draw(Sprite& sprite) {
        requireDrawing();
        if (sprite.textureViews.empty() || !sprite.textureViews[sprite.frameIndex].sdlTexture)
            return;
        sprite.updateVertices();
        append(
            sprite.textureViews[sprite.frameIndex].sdlTexture, sprite.vertices.data(),
            sprite.vertices.size()
        );
    }

}
