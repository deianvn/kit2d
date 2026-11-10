#ifndef KIT2D_SPRITE_SPRITE_BATCH_HPP
#define KIT2D_SPRITE_SPRITE_BATCH_HPP

#include <cstddef>
#include <vector>

#include "Sprite.hpp"
#include "../core/Renderer.hpp"

namespace kit2d {

    class SpriteBatch {
    public:
        explicit SpriteBatch(std::size_t size = 1000);
        SpriteBatch(const SpriteBatch&) = delete;
        SpriteBatch& operator=(const SpriteBatch&) = delete;
        void begin();
        void begin(Renderer& renderer);
        void end();
        void flush();
        bool isDrawing() const;
        void setColor(
            float r,
            float g,
            float b,
            float a = 1.0f
        );
        const SDL_Color& getColor() const;
        void enableBlending();
        void disableBlending();
        bool isBlendingEnabled() const;

        void draw(
            const Texture& texture,
            float x,
            float y
        );
        void draw(
            const Texture& texture,
            float x,
            float y,
            float width,
            float height
        );
        void draw(
            const Texture& texture,
            float x,
            float y,
            int srcX,
            int srcY,
            int srcWidth,
            int srcHeight
        );
        void draw(
            const Texture& texture,
            float x,
            float y,
            float width,
            float height,
            int srcX,
            int srcY,
            int srcWidth,
            int srcHeight,
            bool flipX = false,
            bool flipY = false
        );
        void draw(
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
            bool flipX = false,
            bool flipY = false
        );
        void draw(
            const Texture& texture,
            float x,
            float y,
            float width,
            float height,
            float u,
            float v,
            float u2,
            float v2
        );
        void draw(
            const Texture& texture,
            const std::vector<SDL_Vertex>& vertices,
            std::size_t offset,
            std::size_t count
        );
        void draw(
            const TextureView& region,
            float x,
            float y
        );
        void draw(
            const TextureView& region,
            float x,
            float y,
            float width,
            float height
        );
        void draw(
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
        );
        void draw(Sprite& sprite);

        int renderCalls { 0 };
        int maxSpritesInBatch { 0 };

    private:
        void requireDrawing() const;
        void append(
            SDL_Texture* texture,
            const SDL_Vertex* vertices,
            std::size_t count
        );

        std::size_t capacity;
        Renderer defaultRenderer;
        Renderer* renderer { nullptr };
        SDL_Texture* lastTexture { nullptr };
        std::vector<SDL_Vertex> vertices;
        std::vector<int> indices;
        SDL_Color color { 255, 255, 255, 255 };
        bool blendingEnabled { true };
    };

}

#endif
