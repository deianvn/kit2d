#ifndef KIT2D_CORE_TEXTURE_HPP
#define KIT2D_CORE_TEXTURE_HPP

#include "SDL2.hpp"
#include "Point.hpp"
#include "Rect.hpp"

namespace kit2d {

    class Texture {
    public:
        Texture();
        Texture(const Texture&) = delete;
        Texture& operator=(const Texture&) = delete;
        Texture(Texture&& texture);
        Texture& operator=(Texture&& other);
        ~Texture();
        void loadFromFile(const char* path);
        const Point& getSize() const;

        // Creates a solid-colour render-target texture (requires an active renderer)
        static Texture createSolid(
            int width,
            int height,
            uint8_t r,
            uint8_t g,
            uint8_t b,
            uint8_t a = 255
        );

        SDL_Texture* sdlTexture { nullptr };

    private:
        Point size {};
    };

}

#endif
