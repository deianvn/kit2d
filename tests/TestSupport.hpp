#ifndef KIT2D_TEST_SUPPORT_HPP
#define KIT2D_TEST_SUPPORT_HPP

#include "kit2d/core.hpp"
#include "../src/core/Context.hpp"

#include <array>
#include <memory>
#include <stdexcept>

namespace test {

    inline void check(bool condition, const char* message) {
        if (!condition)
            throw std::runtime_error { message };
    }

    template <typename Exception, typename Function> void checkThrows(Function function) {
        try {
            function();
        } catch (const Exception&) {
            return;
        }
        throw std::runtime_error { "Expected exception was not thrown" };
    }

    class RenderFixture {
        struct SDLGuard {
            SDLGuard() {
                if (SDL_Init(0) != 0)
                    throw kit2d::Err {};
            }
            ~SDLGuard() {
                kit2d::Context::handle = nullptr;
                SDL_Quit();
            }
        } guard;

    public:
        RenderFixture(int width, int height) {
            surface.reset(
                SDL_CreateRGBSurfaceWithFormat(0, width, height, 32, SDL_PIXELFORMAT_RGBA32)
            );
            if (!surface)
                throw kit2d::Err {};
            sdlRenderer.reset(SDL_CreateSoftwareRenderer(surface.get()));
            if (!sdlRenderer)
                throw kit2d::Err {};
            kit2d::Context::handle = sdlRenderer.get();
            clear();
        }

        void clear(
            Uint8 r = 0,
            Uint8 g = 0,
            Uint8 b = 0,
            Uint8 a = 255
        ) {
            renderer.setColor(r, g, b, a);
            renderer.clear();
        }

        SDL_Color pixel(int x, int y) const {
            const SDL_Rect rect { x, y, 1, 1 };
            std::array<Uint8, 4> rgba {};
            if (SDL_RenderReadPixels(
                    sdlRenderer.get(), &rect, SDL_PIXELFORMAT_RGBA32, rgba.data(), 4
                ) != 0)
                throw kit2d::Err {};
            return SDL_Color { rgba[0], rgba[1], rgba[2], rgba[3] };
        }

        std::unique_ptr<SDL_Surface, decltype(&SDL_FreeSurface)> surface { nullptr,
            SDL_FreeSurface };
        std::unique_ptr<SDL_Renderer, decltype(&SDL_DestroyRenderer)> sdlRenderer { nullptr,
            SDL_DestroyRenderer };
        kit2d::Renderer renderer;
    };

}

#endif
