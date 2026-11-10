#include "kit2d/sprite.hpp"
#include "TestSupport.hpp"

#include <array>
#include <iostream>
#include <limits>
#include <stdexcept>

namespace {

    using test::check;
    using test::checkThrows;

    void testAnimation() {
        std::vector<kit2d::TextureView> frames(3);
        frames[1].sdlTextureRect.x = 1;
        frames[2].sdlTextureRect.x = 2;

        kit2d::Sprite empty;
        empty.update(1.0f);
        check(
            empty.getTextureViews().empty() && empty.getFrameIndex() == 0,
            "Default sprites must be empty"
        );

        kit2d::Sprite still { frames[1], 3.0f, 4.0f };
        still.update(10.0f);
        check(
            still.getTextureViews().size() == 1 && still.getFrameIndex() == 0,
            "Single-view sprites must remain static"
        );
        check(
            still.position == kit2d::Vec2 { 3.0f, 4.0f },
            "The positioned static constructor must retain its position"
        );

        kit2d::Sprite defaults { frames };
        check(defaults.getFrameRate() == 12.0f, "Animation must default to 12 frames per second");

        kit2d::Sprite sprite { frames, 8.0f };
        check(sprite.loop && sprite.getFrameRate() == 8.0f, "Animation configuration is incorrect");
        sprite.update(0.0f);
        sprite.update(0.0625f);
        check(sprite.getFrameIndex() == 0, "Frames must not advance before their duration");
        sprite.update(0.0625f);
        check(sprite.getFrameIndex() == 1, "Frames must advance at their exact duration");
        sprite.update(0.3125f);
        check(sprite.getFrameIndex() == 0, "Slow updates must catch up and wrap");
        sprite.update(0.0625f);
        check(sprite.getFrameIndex() == 1, "Updates must preserve fractional-frame progress");
        sprite.update(0.375f);
        check(sprite.getFrameIndex() == 1, "A complete loop must return to the same frame");

        sprite.update(0.0625f);
        sprite.restart();
        check(sprite.getFrameIndex() == 0, "Restarting must return to the first frame");
        sprite.update(0.0625f);
        check(sprite.getFrameIndex() == 0, "Restarting must clear fractional-frame progress");

        sprite.setTextureViews(frames);
        sprite.loop = false;
        sprite.update(0.125f);
        check(sprite.getFrameIndex() == 1, "Non-looping sprites must advance normally");
        sprite.update(10.0f);
        check(sprite.getFrameIndex() == 2, "Non-looping sprites must stop at the last frame");
        sprite.update(10.0f);
        check(sprite.getFrameIndex() == 2, "Completed animations must hold the last frame");
        sprite.loop = true;
        sprite.update(0.0625f);
        check(sprite.getFrameIndex() == 2, "Completed animations must discard excess time");
        sprite.update(0.0625f);
        check(sprite.getFrameIndex() == 0, "Enabling looping must resume a completed animation");

        sprite.update(0.0625f);
        sprite.setFrameRate(16.0f);
        sprite.update(0.03125f);
        check(sprite.getFrameIndex() == 1, "Frame rate changes must preserve fractional progress");

        sprite.update(0.03125f);
        sprite.setTextureViews(frames);
        check(sprite.getFrameIndex() == 0, "Replacing frames must restart the animation");
        sprite.update(0.03125f);
        check(sprite.getFrameIndex() == 0, "Replacing frames must reset fractional progress");
        sprite.update(0.03125f);
        check(
            sprite.getFrameIndex() == 1, "Replacement frames must animate at the configured rate"
        );

        sprite.setTextureView(frames[2]);
        sprite.update(10.0f);
        check(
            sprite.getFrameIndex() == 0 && sprite.getTextureViews().size() == 1 &&
                sprite.getTextureViews()[0].sdlTextureRect.x == 2,
            "Setting a single view must replace the animation with a static sprite"
        );
        sprite.setTextureViews({});
        sprite.update(10.0f);
        check(
            sprite.getTextureViews().empty() && sprite.getFrameIndex() == 0,
            "Clearing frames must produce an empty sprite"
        );

        const float infinity = std::numeric_limits<float>::infinity();
        const float nan = std::numeric_limits<float>::quiet_NaN();
        for (float invalidRate : { 0.0f, -1.0f, infinity, nan }) {
            checkThrows<std::invalid_argument>([&] {
                kit2d::Sprite invalid { frames, invalidRate };
            });
            checkThrows<std::invalid_argument>([&] { sprite.setFrameRate(invalidRate); });
            check(sprite.getFrameRate() == 16.0f, "Invalid rates must not change configuration");
        }
        for (float invalidDelta : { -1.0f, infinity, nan }) {
            checkThrows<std::invalid_argument>([&] { sprite.update(invalidDelta); });
        }

        sprite.setTextureViews(frames);
        sprite.setFrameRate(std::numeric_limits<float>::max());
        sprite.update(std::numeric_limits<float>::max());
        check(
            sprite.getFrameIndex() < frames.size(), "Large updates must keep frame indices valid"
        );
    }

    void testBatchRendering() {
        test::RenderFixture fixture { 2, 1 };

        auto red = kit2d::Texture::createSolid(1, 1, 255, 0, 0);
        auto green = kit2d::Texture::createSolid(1, 1, 0, 255, 0);
        auto blue = kit2d::Texture::createSolid(1, 1, 0, 0, 255);
        kit2d::Sprite sprite { std::vector<kit2d::TextureView> { red, green, blue }, 8.0f };
        sprite.position.x = 1.0f;
        kit2d::Sprite still { red };
        kit2d::Sprite empty;
        auto& renderer = fixture.renderer;
        kit2d::SpriteBatch batch;

        sprite.update(0.125f);
        batch.begin(renderer);
        sprite.draw(batch);
        sprite.draw(batch);
        still.draw(batch);
        empty.draw(batch);
        batch.end();
        check(sprite.getFrameIndex() == 1, "Repeated draws must not advance animation");
        std::array<Uint8, 8> pixels {};
        check(
            SDL_RenderReadPixels(
                fixture.sdlRenderer.get(), nullptr, SDL_PIXELFORMAT_RGBA32, pixels.data(), 8
            ) == 0,
            SDL_GetError()
        );
        check(
            pixels[0] == 255 && pixels[1] == 0 && pixels[2] == 0,
            "Static sprites must still render their texture"
        );
        check(
            pixels[4] == 0 && pixels[5] == 255 && pixels[6] == 0,
            "Animated sprites must render the selected frame at their position"
        );

        fixture.clear();
        batch.begin(renderer);
        batch.end();
        check(sprite.getFrameIndex() == 1, "Empty passes must not update previously drawn sprites");
        check(fixture.pixel(1, 0).g == 0, "Empty passes must not redraw previous geometry");
        sprite.update(0.125f);
        batch.begin(renderer);
        sprite.draw(batch);
        batch.end();
        check(
            sprite.getFrameIndex() == 2,
            "Draw-only batches must not update manually animated sprites"
        );

        sprite.loop = false;
        sprite.update(10.0f);
        batch.begin(renderer);
        sprite.draw(batch);
        batch.end();
        check(
            SDL_RenderReadPixels(
                fixture.sdlRenderer.get(), nullptr, SDL_PIXELFORMAT_RGBA32, pixels.data(), 8
            ) == 0,
            SDL_GetError()
        );
        check(
            pixels[4] == 0 && pixels[5] == 0 && pixels[6] == 255,
            "Non-looping sprites must draw their final texture after completion"
        );
    }

}

int main() {
    try {
        testAnimation();
        testBatchRendering();
    } catch (const std::exception& error) {
        std::cerr << error.what() << '\n';
        return 1;
    }
    return 0;
}
