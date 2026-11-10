#ifndef KIT2D_SPRITE_SPRITE_HPP
#define KIT2D_SPRITE_SPRITE_HPP

#include <array>
#include <cstddef>
#include <vector>

#include "../core/TextureView.hpp"
#include "../core/Renderer.hpp"
#include "Vec2.hpp"

namespace kit2d {

    class SpriteBatch;

    class Sprite {
    public:
        Sprite();
        Sprite(TextureView textureView) : textureViews { textureView } {
        }
        Sprite(
            TextureView textureView,
            float x,
            float y
        )
            : position(x, y), textureViews { textureView } {
        }
        Sprite(std::vector<TextureView> textureViews, float frameRate = 12.0f);
        void setTextureView(TextureView textureView);
        void setTextureViews(std::vector<TextureView> textureViews);
        const std::vector<TextureView>& getTextureViews() const;
        void setFrameRate(float frameRate);
        float getFrameRate() const;
        std::size_t getFrameIndex() const;
        void setPosition(float x, float y);
        void setSize(float width, float height);
        float getWidth() const;
        float getHeight() const;
        void setOrigin(float x, float y);
        void setScale(float x, float y);
        void setRotation(float degrees);
        void setColor(
            float r,
            float g,
            float b,
            float a = 1.0f
        );
        const SDL_Color& getColor() const;
        void restart();
        void update(float deltaTime);
        void draw(SpriteBatch& batch);
        void draw(Renderer& renderer);

        Vec2 position;
        bool loop { true };

    private:
        friend class SpriteBatch;
        void updateVertices();

        std::vector<TextureView> textureViews;
        float frameRate { 12.0f };
        std::size_t frameIndex { 0 };
        double frameProgress { 0.0 };
        Vec2 size;
        Vec2 origin;
        Vec2 scale { 1.0f, 1.0f };
        float rotation { 0.0f };
        SDL_Color color { 255, 255, 255, 255 };
        bool customSize { false };
        bool verticesDirty { true };
        Vec2 cachedPosition;
        std::array<SDL_Vertex, 4> vertices {};
    };

}

#endif
