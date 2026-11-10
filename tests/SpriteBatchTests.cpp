#include "kit2d/sprite.hpp"
#include "TestSupport.hpp"

#include <cmath>
#include <iostream>
#include <limits>
#include <vector>

namespace {

    using test::check;
    using test::checkThrows;

    void checkColor(
        SDL_Color actual,
        SDL_Color expected,
        const char* message,
        int tolerance = 0
    ) {
        check(
            std::abs(actual.r - expected.r) <= tolerance &&
                std::abs(actual.g - expected.g) <= tolerance &&
                std::abs(actual.b - expected.b) <= tolerance &&
                std::abs(actual.a - expected.a) <= tolerance,
            message
        );
    }

    void testLifecycle(test::RenderFixture& fixture) {
        checkThrows<std::invalid_argument>([] { kit2d::SpriteBatch invalid { 0 }; });
        checkThrows<std::invalid_argument>([] { kit2d::SpriteBatch invalid { 8192 }; });
        kit2d::SpriteBatch largest { 8191 };
        kit2d::SpriteBatch batch;
        auto white = kit2d::Texture::createSolid(1, 1, 255, 255, 255);
        kit2d::Sprite empty;
        checkThrows<std::logic_error>([&] { batch.draw(white, 0.0f, 0.0f); });
        checkThrows<std::logic_error>([&] { empty.draw(batch); });
        checkThrows<std::logic_error>([&] { batch.end(); });
        checkThrows<std::logic_error>([&] { batch.flush(); });
        check(!batch.isDrawing(), "New batches must not be drawing");

        batch.begin(fixture.renderer);
        check(batch.isDrawing(), "begin must start drawing");
        checkThrows<std::logic_error>([&] { batch.begin(); });
        checkThrows<std::logic_error>([&] { batch.begin(fixture.renderer); });
        batch.end();
        check(
            !batch.isDrawing() && batch.renderCalls == 0, "Empty batches must submit no geometry"
        );

        fixture.clear();
        batch.begin();
        batch.draw(white, 0.0f, 0.0f);
        batch.end();
        checkColor(
            fixture.pixel(0, 0), { 255, 255, 255, 255 },
            "Parameterless begin must use the active renderer"
        );

        batch.begin(fixture.renderer);
        for (int i = 0; i < 1001; ++i)
            batch.draw(white, 40.0f, 0.0f);
        batch.end();
        check(
            batch.renderCalls == 2 && batch.maxSpritesInBatch == 1000,
            "The default buffer must hold exactly 1000 quads"
        );
        largest.begin(fixture.renderer);
        for (int i = 0; i < 8192; ++i)
            largest.draw(white, 40.0f, 0.0f);
        largest.end();
        check(
            largest.renderCalls == 2 && largest.maxSpritesInBatch == 8191,
            "The maximum supported capacity must submit 8191 quads safely"
        );
    }

    void testBatching(test::RenderFixture& fixture) {
        fixture.clear();
        auto red = kit2d::Texture::createSolid(1, 1, 255, 0, 0);
        auto green = kit2d::Texture::createSolid(1, 1, 0, 255, 0);
        kit2d::SpriteBatch batch { 2 };
        batch.begin(fixture.renderer);
        batch.draw(red, 0.0f, 0.0f);
        batch.draw(red, 1.0f, 0.0f);
        check(batch.renderCalls == 0, "Drawing must buffer geometry until a flush is needed");
        checkColor(
            fixture.pixel(0, 0), { 0, 0, 0, 255 }, "Buffered geometry must not draw immediately"
        );
        batch.draw(red, 2.0f, 0.0f);
        check(
            batch.renderCalls == 1 && batch.maxSpritesInBatch == 2,
            "A full buffer must flush at its configured capacity"
        );
        batch.draw(green, 3.0f, 0.0f);
        check(batch.renderCalls == 2, "Changing textures must flush buffered geometry");
        batch.draw(red, 4.0f, 0.0f);
        batch.end();
        check(
            batch.renderCalls == 4 && batch.maxSpritesInBatch == 2,
            "Statistics must count actual geometry submissions"
        );
        checkColor(
            fixture.pixel(3, 0), { 0, 255, 0, 255 }, "Texture changes must draw the correct texture"
        );

        batch.begin(fixture.renderer);
        check(
            batch.renderCalls == 0 && batch.maxSpritesInBatch == 2,
            "begin must reset renderCalls but preserve lifetime peak capacity"
        );
        batch.draw(red, 0.0f, 0.0f);
        batch.draw(green, 0.0f, 0.0f);
        batch.draw(red, 0.0f, 0.0f);
        batch.end();
        check(batch.renderCalls == 3, "Non-consecutive textures must not be globally reordered");
        checkColor(fixture.pixel(0, 0), { 255, 0, 0, 255 }, "Batching must preserve painter order");

        batch.maxSpritesInBatch = 0;
        batch.begin(fixture.renderer);
        batch.draw(red, 0.0f, 0.0f);
        batch.flush();
        batch.flush();
        check(
            batch.isDrawing() && batch.renderCalls == 1 && batch.maxSpritesInBatch == 1,
            "Explicit flush must keep drawing active and ignore empty buffers"
        );
        batch.end();
    }

    void testRegions(test::RenderFixture& fixture) {
        fixture.clear();
        auto atlas = kit2d::Texture::createSolid(2, 2, 255, 255, 255);
        fixture.renderer.setTarget(&atlas);
        fixture.renderer.setColor(255, 0, 0, 255);
        fixture.renderer.fillRect({ 0, 0, 1, 1 });
        fixture.renderer.setColor(0, 255, 0, 255);
        fixture.renderer.fillRect({ 1, 0, 1, 1 });
        fixture.renderer.setColor(0, 0, 255, 255);
        fixture.renderer.fillRect({ 0, 1, 1, 1 });
        fixture.renderer.setTarget();
        kit2d::TextureView white { atlas, { 1, 1, 1, 1 } };
        kit2d::SpriteBatch batch;
        batch.begin(fixture.renderer);
        batch.draw(atlas, 0.0f, 0.0f);
        batch.draw(atlas, 4.0f, 0.0f, 1, 0, 1, 1);
        batch.draw(atlas, 6.0f, 0.0f, 2.0f, 2.0f, 1, 0, 1, 1);
        batch.draw(atlas, 10.0f, 0.0f, 2.0f, 2.0f, 0, 0, 2, 2, true, false);
        batch.draw(atlas, 14.0f, 0.0f, 2.0f, 2.0f, 0, 0, 2, 2, false, true);
        batch.draw(atlas, 18.0f, 0.0f, 2.0f, 2.0f, 0.5f, 0.0f, 1.0f, 0.5f);
        batch.draw(white, 0.0f, 4.0f);
        batch.draw(white, 4.0f, 4.0f, 3.0f, 2.0f);
        batch.end();
        check(
            batch.renderCalls == 1 && batch.maxSpritesInBatch == 8,
            "Different regions of one atlas must share a geometry submission"
        );
        checkColor(fixture.pixel(0, 0), { 255, 0, 0, 255 }, "Full textures must retain their UVs");
        checkColor(
            fixture.pixel(4, 0), { 0, 255, 0, 255 },
            "Source rectangles must select the requested pixels"
        );
        checkColor(
            fixture.pixel(7, 1), { 0, 255, 0, 255 }, "Source rectangles must support stretching"
        );
        checkColor(
            fixture.pixel(10, 0), { 0, 255, 0, 255 },
            "Horizontal flipping must reverse U coordinates"
        );
        checkColor(
            fixture.pixel(14, 0), { 0, 0, 255, 255 }, "Vertical flipping must reverse V coordinates"
        );
        checkColor(
            fixture.pixel(19, 1), { 0, 255, 0, 255 },
            "Normalized UV rectangles must select texture regions"
        );
        checkColor(
            fixture.pixel(0, 4), { 255, 255, 255, 255 },
            "Texture views must use their natural dimensions"
        );
        checkColor(
            fixture.pixel(6, 5), { 255, 255, 255, 255 },
            "Texture views must support explicit dimensions"
        );

        batch.begin(fixture.renderer);
        checkThrows<std::invalid_argument>([&] {
            batch.draw(kit2d::TextureView { atlas, { 1, 1, 2, 2 } }, 0.0f, 0.0f);
        });
        checkThrows<std::invalid_argument>([&] { batch.draw(atlas, 0.0f, 0.0f, -1.0f, 1.0f); });
        checkThrows<std::invalid_argument>([&] {
            batch.draw(atlas, std::numeric_limits<float>::infinity(), 0.0f);
        });
        checkThrows<std::invalid_argument>([&] {
            batch.draw(atlas, 0.0f, 0.0f, 1.0f, 1.0f, -0.5f, 0.0f, 1.0f, 1.0f);
        });
        batch.end();
    }

    void testTransformsAndRawVertices(test::RenderFixture& fixture) {
        fixture.clear();
        auto white = kit2d::Texture::createSolid(1, 1, 255, 255, 255);
        kit2d::TextureView region { white };
        kit2d::SpriteBatch batch;
        batch.begin(fixture.renderer);
        batch.draw(white, 8.0f, 4.0f, 2.0f, 1.0f, 4.0f, 2.0f, 1.0f, 1.0f, 90.0f, 0, 0, 1, 1);
        batch.draw(region, 1.0f, 9.0f, 0.0f, 0.0f, 2.0f, 1.0f, 2.0f, 3.0f, 0.0f);
        batch.draw(region, 18.0f, 4.0f, 0.0f, 0.0f, 2.0f, 2.0f, -1.0f, 1.0f, 0.0f);
        batch.end();
        checkColor(
            fixture.pixel(10, 4), { 255, 255, 255, 255 },
            "Rotation must transform geometry around the requested local origin"
        );
        checkColor(
            fixture.pixel(8, 4), { 0, 0, 0, 255 },
            "Rotated quads must not retain their original bounds"
        );
        checkColor(
            fixture.pixel(4, 11), { 255, 255, 255, 255 },
            "Scale must apply independently on each axis"
        );
        checkColor(
            fixture.pixel(5, 11), { 0, 0, 0, 255 }, "Scaled geometry must use the requested bounds"
        );
        checkColor(
            fixture.pixel(16, 4), { 255, 255, 255, 255 }, "Negative scales must mirror geometry"
        );
        checkColor(
            fixture.pixel(18, 4), { 0, 0, 0, 255 },
            "Mirrored geometry must remain relative to its origin"
        );

        std::vector<SDL_Vertex> vertices(8);
        vertices[4] = { { 25.0f, 2.0f }, { 0, 255, 255, 255 }, { 0.0f, 0.0f } };
        vertices[5] = { { 28.0f, 5.0f }, { 0, 255, 255, 255 }, { 1.0f, 0.0f } };
        vertices[6] = { { 25.0f, 8.0f }, { 0, 255, 255, 255 }, { 1.0f, 1.0f } };
        vertices[7] = { { 22.0f, 5.0f }, { 0, 255, 255, 255 }, { 0.0f, 1.0f } };
        batch.setColor(1.0f, 0.0f, 0.0f);
        batch.begin(fixture.renderer);
        batch.draw(white, vertices, 4, 4);
        checkThrows<std::invalid_argument>([&] { batch.draw(white, vertices, 9, 0); });
        checkThrows<std::invalid_argument>([&] { batch.draw(white, vertices, 4, 8); });
        checkThrows<std::invalid_argument>([&] { batch.draw(white, vertices, 4, 3); });
        batch.end();
        checkColor(
            fixture.pixel(25, 5), { 0, 255, 255, 255 },
            "Raw quads must preserve supplied geometry, colors, UVs and vertex offsets"
        );
        checkColor(
            fixture.pixel(22, 2), { 0, 0, 0, 255 }, "Raw quads need not be axis-aligned rectangles"
        );

        kit2d::SpriteBatch small { 1 };
        small.begin(fixture.renderer);
        small.draw(white, vertices, 0, vertices.size());
        small.end();
        check(
            small.renderCalls == 2 && small.maxSpritesInBatch == 1,
            "Raw quads must obey the configured buffer capacity"
        );
    }

    void testTintAndBlending(test::RenderFixture& fixture) {
        fixture.clear(0, 0, 255);
        auto white = kit2d::Texture::createSolid(1, 1, 255, 255, 255);
        if (SDL_SetTextureBlendMode(white.sdlTexture, SDL_BLENDMODE_ADD) != 0)
            throw kit2d::Err {};
        if (SDL_SetTextureColorMod(white.sdlTexture, 1, 1, 1) != 0)
            throw kit2d::Err {};
        if (SDL_SetTextureAlphaMod(white.sdlTexture, 1) != 0)
            throw kit2d::Err {};
        kit2d::SpriteBatch batch;
        check(batch.isBlendingEnabled(), "Blending must be enabled by default");
        batch.begin(fixture.renderer);
        batch.setColor(1.0f, 0.0f, 0.0f, 0.5f);
        batch.draw(white, 0.0f, 0.0f);
        batch.disableBlending();
        check(batch.renderCalls == 1, "Blend-state changes must flush pending geometry first");
        batch.draw(white, 1.0f, 0.0f);
        batch.enableBlending();
        batch.setColor(0.0f, 1.0f, 0.0f);
        batch.draw(white, 2.0f, 0.0f);
        batch.setColor(0.0f, 0.0f, 1.0f);
        batch.draw(white, 3.0f, 0.0f);
        batch.end();
        check(batch.renderCalls == 3, "Tint changes must not break batches but blend changes must");
        checkColor(
            fixture.pixel(0, 0), { 128, 0, 127, 255 },
            "Enabled blending must composite vertex alpha", 1
        );
        checkColor(
            fixture.pixel(1, 0), { 255, 0, 0, 128 },
            "Disabled blending must replace destination pixels", 1
        );
        checkColor(
            fixture.pixel(2, 0), { 0, 255, 0, 255 },
            "Tint must be captured per quad, independent of texture mods"
        );
        checkColor(
            fixture.pixel(3, 0), { 0, 0, 255, 255 }, "Changing tint must not recolor earlier quads"
        );
        SDL_BlendMode mode;
        if (SDL_GetTextureBlendMode(white.sdlTexture, &mode) != 0)
            throw kit2d::Err {};
        check(
            mode == SDL_BLENDMODE_ADD, "Geometry submissions must restore the texture blend mode"
        );
        checkThrows<std::invalid_argument>([&] { batch.setColor(2.0f, 0.0f, 0.0f); });
        checkThrows<std::invalid_argument>([&] {
            batch.setColor(std::numeric_limits<float>::quiet_NaN(), 0.0f, 0.0f);
        });
        check(batch.getColor().b == 255, "Invalid colors must not change batch state");
        checkThrows<kit2d::Err>([&] {
            fixture.renderer.drawGeometry(white.sdlTexture, nullptr, 4, nullptr, 6);
        });
        if (SDL_GetTextureBlendMode(white.sdlTexture, &mode) != 0)
            throw kit2d::Err {};
        check(
            mode == SDL_BLENDMODE_ADD, "Failed geometry draws must also restore texture blend state"
        );
    }

    void testSprites(test::RenderFixture& fixture) {
        fixture.clear();
        auto white = kit2d::Texture::createSolid(1, 1, 255, 255, 255);
        auto green = kit2d::Texture::createSolid(1, 1, 0, 255, 0);
        kit2d::Sprite sprite { white };
        sprite.setPosition(2.0f, 2.0f);
        sprite.setSize(2.0f, 1.0f);
        sprite.setOrigin(1.0f, 0.5f);
        sprite.setScale(1.0f, 2.0f);
        sprite.setRotation(90.0f);
        sprite.setColor(0.0f, 0.0f, 1.0f);
        kit2d::SpriteBatch batch;
        batch.setColor(1.0f, 0.0f, 0.0f);
        batch.begin(fixture.renderer);
        sprite.draw(batch);
        sprite.position.x = 6.0f;
        sprite.draw(batch);
        batch.end();
        checkColor(
            fixture.pixel(2, 2), { 0, 0, 255, 255 },
            "Sprites must use their own tint and transforms"
        );
        checkColor(
            fixture.pixel(6, 2), { 0, 0, 255, 255 },
            "Direct position changes must invalidate cached geometry"
        );
        check(
            sprite.getWidth() == 2.0f && sprite.getHeight() == 1.0f && sprite.getColor().b == 255,
            "Sprite properties must retain their configured values"
        );

        sprite.setTextureView(green);
        sprite.setColor(1.0f, 1.0f, 1.0f);
        sprite.setPosition(10.0f, 2.0f);
        sprite.draw(fixture.renderer);
        checkColor(
            fixture.pixel(10, 2), { 0, 255, 0, 255 },
            "Immediate sprite drawing must honor updated texture, color and geometry"
        );
        checkThrows<std::invalid_argument>([&] { sprite.setSize(-1.0f, 1.0f); });
        checkThrows<std::invalid_argument>([&] {
            sprite.setRotation(std::numeric_limits<float>::infinity());
        });
        checkThrows<std::invalid_argument>([&] {
            sprite.setScale(1.0f, std::numeric_limits<float>::quiet_NaN());
        });

        kit2d::Sprite animated { std::vector<kit2d::TextureView> { white, green }, 8.0f };
        animated.update(0.125f);
        batch.begin(fixture.renderer);
        animated.draw(batch);
        batch.flush();
        animated.position.x = 1.0f;
        animated.draw(batch);
        batch.end();
        check(animated.getFrameIndex() == 1, "Drawing and flushing must not advance animation");
        checkColor(
            fixture.pixel(0, 0), { 0, 255, 0, 255 },
            "Animated sprites must draw their selected texture"
        );
        checkColor(
            fixture.pixel(1, 0), { 0, 255, 0, 255 },
            "Repeated submissions must draw the same animation frame"
        );
        batch.begin(fixture.renderer);
        animated.draw(batch);
        batch.end();
        check(animated.getFrameIndex() == 1, "New render passes must not advance animation");
        animated.update(0.125f);
        batch.begin(fixture.renderer);
        animated.draw(batch);
        batch.end();
        check(animated.getFrameIndex() == 0, "Only explicit updates must advance animation");
    }

}

int main() {
    try {
        test::RenderFixture fixture { 32, 16 };
        testLifecycle(fixture);
        testBatching(fixture);
        testRegions(fixture);
        testTransformsAndRawVertices(fixture);
        testTintAndBlending(fixture);
        testSprites(fixture);
    } catch (const std::exception& error) {
        std::cerr << error.what() << '\n';
        return 1;
    }
    return 0;
}
