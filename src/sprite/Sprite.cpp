#include "../../include/kit2d/sprite/Sprite.hpp"
#include "../../include/kit2d/sprite/SpriteBatch.hpp"
#include "Geometry.hpp"

#include <cmath>
#include <stdexcept>
#include <utility>

namespace kit2d {

    Sprite::Sprite() {
    }

    Sprite::Sprite(std::vector<TextureView> textureViews, float frameRate)
        : textureViews(std::move(textureViews)) {
        setFrameRate(frameRate);
    }

    void Sprite::setTextureView(TextureView textureView) {
        setTextureViews({ textureView });
    }

    void Sprite::setTextureViews(std::vector<TextureView> textureViews) {
        this->textureViews = std::move(textureViews);
        restart();
    }

    const std::vector<TextureView>& Sprite::getTextureViews() const {
        return textureViews;
    }

    void Sprite::setFrameRate(float frameRate) {
        if (!std::isfinite(frameRate) || frameRate <= 0.0f) {
            throw std::invalid_argument { "Sprite frame rate must be finite and positive" };
        }
        this->frameRate = frameRate;
    }

    float Sprite::getFrameRate() const {
        return frameRate;
    }

    std::size_t Sprite::getFrameIndex() const {
        return frameIndex;
    }

    void Sprite::setPosition(float x, float y) {
        detail::requireFinite({ x, y });
        position = Vec2 { x, y };
    }

    void Sprite::setSize(float width, float height) {
        detail::requireFinite({ width, height });
        if (width < 0.0f || height < 0.0f) {
            throw std::invalid_argument { "Sprite dimensions must be non-negative" };
        }
        if (!customSize || size != Vec2 { width, height }) {
            size = Vec2 { width, height };
            customSize = true;
            verticesDirty = true;
        }
    }

    float Sprite::getWidth() const {
        if (customSize)
            return size.x;
        return textureViews.empty() ? 0.0f
                                    : static_cast<float>(textureViews[frameIndex].sdlTextureRect.w);
    }

    float Sprite::getHeight() const {
        if (customSize)
            return size.y;
        return textureViews.empty() ? 0.0f
                                    : static_cast<float>(textureViews[frameIndex].sdlTextureRect.h);
    }

    void Sprite::setOrigin(float x, float y) {
        detail::requireFinite({ x, y });
        if (origin != Vec2 { x, y }) {
            origin = Vec2 { x, y };
            verticesDirty = true;
        }
    }

    void Sprite::setScale(float x, float y) {
        detail::requireFinite({ x, y });
        if (scale != Vec2 { x, y }) {
            scale = Vec2 { x, y };
            verticesDirty = true;
        }
    }

    void Sprite::setRotation(float degrees) {
        detail::requireFinite({ degrees });
        if (rotation != degrees) {
            rotation = degrees;
            verticesDirty = true;
        }
    }

    void Sprite::setColor(
        float r,
        float g,
        float b,
        float a
    ) {
        const auto newColor = detail::makeColor(r, g, b, a);
        if (color.r != newColor.r || color.g != newColor.g || color.b != newColor.b ||
            color.a != newColor.a) {
            color = newColor;
            verticesDirty = true;
        }
    }

    const SDL_Color& Sprite::getColor() const {
        return color;
    }

    void Sprite::restart() {
        frameIndex = 0;
        frameProgress = 0.0;
        verticesDirty = true;
    }

    void Sprite::update(float deltaTime) {
        if (!std::isfinite(deltaTime) || deltaTime < 0.0f) {
            throw std::invalid_argument { "Sprite delta time must be finite and non-negative" };
        }
        if (textureViews.size() < 2 || (!loop && frameIndex == textureViews.size() - 1)) {
            return;
        }

        frameProgress += static_cast<double>(deltaTime) * frameRate;
        if (frameProgress < 1.0)
            return;
        const double framesToAdvance = std::floor(frameProgress);
        frameProgress -= framesToAdvance;
        const auto previousIndex = frameIndex;

        if (loop) {
            const auto advance = static_cast<std::size_t>(
                std::fmod(framesToAdvance, static_cast<double>(textureViews.size()))
            );
            frameIndex = (frameIndex + advance) % textureViews.size();
        } else {
            const auto remaining = textureViews.size() - 1 - frameIndex;
            if (framesToAdvance >= static_cast<double>(remaining)) {
                frameIndex = textureViews.size() - 1;
                frameProgress = 0.0;
            } else {
                frameIndex += static_cast<std::size_t>(framesToAdvance);
            }
        }
        if (frameIndex != previousIndex)
            verticesDirty = true;
    }

    void Sprite::updateVertices() {
        if (!verticesDirty && cachedPosition == position)
            return;
        vertices = detail::makeQuad(
            textureViews[frameIndex], position.x, position.y, origin.x, origin.y, getWidth(),
            getHeight(), scale.x, scale.y, rotation, color
        );
        cachedPosition = position;
        verticesDirty = false;
    }

    void Sprite::draw(SpriteBatch& batch) {
        batch.draw(*this);
    }

    void Sprite::draw(Renderer& renderer) {
        if (textureViews.empty() || !textureViews[frameIndex].sdlTexture)
            return;
        updateVertices();
        const int indices[] { 0, 1, 2, 2, 3, 0 };
        renderer.drawGeometry(
            textureViews[frameIndex].sdlTexture, vertices.data(), static_cast<int>(vertices.size()),
            indices, 6
        );
    }

}
