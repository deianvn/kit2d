#ifndef KIT2D_CORE_RENDERER_HPP
#define KIT2D_CORE_RENDERER_HPP

#include "SDL2.hpp"
#include "Rect.hpp"
#include "Texture.hpp"
#include "TextureView.hpp"

namespace kit2d {

    class Renderer {
    public:
        Renderer();
        virtual ~Renderer();
        void setTarget(Texture* texture = nullptr);
        void setClipRect(const Rect* rect = nullptr);
        void setColor(
            uint8_t r,
            uint8_t g,
            uint8_t b,
            uint8_t a
        );
        void clear();
        void draw(
            Texture& texture,
            int x,
            int y
        );
        void draw(
            TextureView textureView,
            int x,
            int y
        );
        void drawGeometry(
            SDL_Texture* texture,
            const SDL_Vertex* vertices,
            int vertexCount,
            const int* indices,
            int indexCount,
            SDL_BlendMode blendMode = SDL_BLENDMODE_BLEND
        );
        void drawPoint(int x, int y);
        void drawLine(
            int x1,
            int y1,
            int x2,
            int y2
        );
        void drawRect(const Rect& rect);
        void fillRect(const Rect& rect);
        void present();

    private:
        SDL_Rect destRect {};
    };

}

#endif
