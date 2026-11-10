#ifndef KIT2D_SPRITE_GEOMETRY_HPP
#define KIT2D_SPRITE_GEOMETRY_HPP

#include "../../include/kit2d/core/Err.hpp"
#include "../../include/kit2d/core/TextureView.hpp"

#include <array>
#include <cmath>
#include <initializer_list>
#include <stdexcept>

namespace kit2d {
    namespace detail {

        inline void requireFinite(std::initializer_list<float> values) {
            for (float value : values) {
                if (!std::isfinite(value)) {
                    throw std::invalid_argument { "Sprite geometry must be finite" };
                }
            }
        }

        inline SDL_Color makeColor(
            float r,
            float g,
            float b,
            float a
        ) {
            requireFinite({ r, g, b, a });
            for (float value : { r, g, b, a }) {
                if (value < 0.0f || value > 1.0f) {
                    throw std::invalid_argument { "Color components must be between zero and one" };
                }
            }
            return SDL_Color { static_cast<Uint8>(std::lround(r * 255.0f)),
                static_cast<Uint8>(std::lround(g * 255.0f)),
                static_cast<Uint8>(std::lround(b * 255.0f)),
                static_cast<Uint8>(std::lround(a * 255.0f)) };
        }

        inline std::array<SDL_Vertex, 4> makeQuad(
            float x,
            float y,
            float originX,
            float originY,
            float width,
            float height,
            float scaleX,
            float scaleY,
            float rotation,
            float u,
            float v,
            float u2,
            float v2,
            SDL_Color color
        ) {

            requireFinite(
                { x, y, originX, originY, width, height, scaleX, scaleY, rotation, u, v, u2, v2 }
            );
            if (width < 0.0f || height < 0.0f) {
                throw std::invalid_argument { "Sprite dimensions must be non-negative" };
            }
            for (float value : { u, v, u2, v2 }) {
                if (value < 0.0f || value > 1.0f) {
                    throw std::invalid_argument {
                        "Texture coordinates must be between zero and one"
                    };
                }
            }

            const float radians = std::fmod(rotation, 360.0f) * 0.017453292519943295f;
            const float cosine = std::cos(radians);
            const float sine = std::sin(radians);
            const std::array<SDL_FPoint, 4> corners { { { 0.0f, 0.0f }, { width, 0.0f },
                { width, height }, { 0.0f, height } } };
            const std::array<SDL_FPoint, 4> coordinates { { { u, v }, { u2, v }, { u2, v2 },
                { u, v2 } } };
            std::array<SDL_Vertex, 4> vertices {};
            for (std::size_t i = 0; i < vertices.size(); ++i) {
                const float localX = (corners[i].x - originX) * scaleX;
                const float localY = (corners[i].y - originY) * scaleY;
                const float worldX = x + originX + localX * cosine - localY * sine;
                const float worldY = y + originY + localX * sine + localY * cosine;
                requireFinite({ worldX, worldY });
                vertices[i] = SDL_Vertex { { worldX, worldY }, color, coordinates[i] };
            }
            return vertices;
        }

        inline std::array<SDL_Vertex, 4> makeQuad(
            const TextureView& region,
            float x,
            float y,
            float originX,
            float originY,
            float width,
            float height,
            float scaleX,
            float scaleY,
            float rotation,
            SDL_Color color,
            bool flipX = false,
            bool flipY = false
        ) {

            int textureWidth, textureHeight;
            if (SDL_QueryTexture(
                    region.sdlTexture, nullptr, nullptr, &textureWidth, &textureHeight
                ) != 0) {
                throw Err {};
            }
            const auto& rect = region.sdlTextureRect;
            if (rect.x < 0 || rect.y < 0 || rect.w < 0 || rect.h < 0 || rect.w > textureWidth ||
                rect.h > textureHeight || rect.x > textureWidth - rect.w ||
                rect.y > textureHeight - rect.h) {
                throw std::invalid_argument { "Texture region must be inside its texture" };
            }
            const float u = static_cast<float>(rect.x) / textureWidth;
            const float v = static_cast<float>(rect.y) / textureHeight;
            const float u2 = static_cast<float>(rect.x + rect.w) / textureWidth;
            const float v2 = static_cast<float>(rect.y + rect.h) / textureHeight;
            return makeQuad(
                x, y, originX, originY, width, height, scaleX, scaleY, rotation, flipX ? u2 : u,
                flipY ? v2 : v, flipX ? u : u2, flipY ? v : v2, color
            );
        }

    }
}

#endif
