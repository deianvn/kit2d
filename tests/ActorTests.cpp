#include "kit2d/scene.hpp"
#include "TestSupport.hpp"

#include <iostream>
#include <limits>
#include <vector>

namespace {

    using test::check;
    using test::checkThrows;

    class MovingActor : public kit2d::Actor {
    public:
        using Actor::Actor;

        void act(float deltaTime) override {
            Actor::act(deltaTime);
            ++updates;
            setPosition(getPosition().x + 8.0f * deltaTime, getPosition().y);
        }

        int updates { 0 };
    };

    void testUpdatesAndSwitching() {
        std::vector<kit2d::TextureView> frames(3);
        frames[1].sdlTextureRect.x = 1;
        frames[2].sdlTextureRect.x = 2;

        kit2d::Actor empty;
        empty.act(1.0f);
        check(
            empty.getSprite().getTextureViews().empty(), "Default actors must have an empty sprite"
        );
        empty.setPosition(5.0f, 6.0f);
        empty.setSprite(kit2d::Sprite { frames, 8.0f });
        check(
            empty.getPosition() == kit2d::Vec2 { 5.0f, 6.0f },
            "Assigning the first sprite must preserve the actor position"
        );

        kit2d::Sprite source { frames, 8.0f };
        source.setPosition(12.0f, 34.0f);
        source.loop = false;
        source.update(0.1875f);
        kit2d::Actor actor { source };
        check(
            actor.getSprite().getFrameIndex() == 0 && !actor.getSprite().loop,
            "New actors must restart their sprite while retaining its configuration"
        );
        check(
            actor.getPosition() == source.position,
            "Construction must retain the initial sprite position"
        );
        check(
            source.getFrameIndex() == 1, "Constructing an actor must not mutate the source sprite"
        );
        actor.act(0.0625f);
        check(
            actor.getSprite().getFrameIndex() == 0,
            "New actors must discard the source's fractional progress"
        );
        actor.act(0.0625f);
        check(
            actor.getSprite().getFrameIndex() == 1,
            "Actor updates must advance the current sprite in seconds"
        );
        actor.act(10.0f);
        check(
            actor.getSprite().getFrameIndex() == 2,
            "Actors must honor non-looping sprite animations"
        );

        kit2d::Sprite otherDirection {
            std::vector<kit2d::TextureView> { frames[2], frames[1], frames[0] }, 4.0f
        };
        otherDirection.setPosition(99.0f, 99.0f);
        otherDirection.update(0.375f);
        actor.setPosition(5.0f, 6.0f);
        actor.setSprite(otherDirection);
        check(
            actor.getPosition() == kit2d::Vec2 { 5.0f, 6.0f },
            "Changing direction must not teleport the actor to the new sprite's position"
        );
        check(
            actor.getSprite().getFrameIndex() == 0 &&
                actor.getSprite().getTextureViews()[0].sdlTextureRect.x == 2,
            "Changing sprites must select the new sequence and restart from its first frame"
        );
        check(
            actor.getSprite().getFrameRate() == 4.0f && actor.getSprite().loop,
            "Changing sprites must use the new frame rate and loop configuration"
        );
        actor.act(0.125f);
        check(
            actor.getSprite().getFrameIndex() == 0,
            "Changing sprites must clear fractional progress"
        );
        actor.act(0.125f);
        check(
            actor.getSprite().getFrameIndex() == 1,
            "The replacement sprite must animate at its own rate"
        );
        const kit2d::Actor& constActor = actor;
        check(
            constActor.getSprite().getFrameIndex() == 1,
            "Const actors must expose their current sprite"
        );
        check(
            otherDirection.getFrameIndex() == 1,
            "Actors must own independent copies of supplied sprites"
        );
        kit2d::Actor second { otherDirection };
        actor.act(0.25f);
        check(
            second.getSprite().getFrameIndex() == 0,
            "Actors using the same template must animate independently"
        );

        for (float invalidDelta : { -1.0f, std::numeric_limits<float>::infinity(),
                 std::numeric_limits<float>::quiet_NaN() }) {
            checkThrows<std::invalid_argument>([&] { actor.act(invalidDelta); });
        }
        checkThrows<std::invalid_argument>([&] {
            actor.setPosition(std::numeric_limits<float>::infinity(), 0.0f);
        });
        check(
            actor.getPosition() == kit2d::Vec2 { 5.0f, 6.0f },
            "Invalid positions must leave the actor unchanged"
        );

        MovingActor moving { kit2d::Sprite { frames, 8.0f } };
        kit2d::Actor& base = moving;
        base.act(0.125f);
        check(
            moving.updates == 1 && moving.getSprite().getFrameIndex() == 1 &&
                moving.getPosition().x == 1.0f,
            "Actor updates must dispatch to overridden act implementations"
        );
    }

    void testDrawing() {
        test::RenderFixture fixture { 4, 2 };
        auto red = kit2d::Texture::createSolid(1, 1, 255, 0, 0);
        auto green = kit2d::Texture::createSolid(1, 1, 0, 255, 0);
        auto blue = kit2d::Texture::createSolid(1, 1, 0, 0, 255);
        kit2d::Sprite walkRight { std::vector<kit2d::TextureView> { red, green, blue }, 8.0f };
        kit2d::Sprite walkLeft { std::vector<kit2d::TextureView> { blue, red }, 8.0f };
        kit2d::Actor actor { walkRight };
        actor.setPosition(2.0f, 1.0f);
        kit2d::SpriteBatch batch;
        checkThrows<std::logic_error>([&] { actor.draw(batch); });
        actor.act(0.125f);
        batch.begin(fixture.renderer);
        actor.draw(batch);
        batch.flush();
        actor.draw(batch);
        batch.end();
        check(
            actor.getSprite().getFrameIndex() == 1,
            "Repeated actor draws must not advance animation"
        );
        check(
            fixture.pixel(2, 1).g == 255,
            "Actors must draw their current sprite at the actor position"
        );
        check(
            walkRight.getFrameIndex() == 0,
            "Drawing an actor must not mutate its animation template"
        );

        fixture.clear();
        actor.act(0.125f);
        check(
            actor.getSprite().getFrameIndex() == 2,
            "Actors must animate even when not submitted for drawing"
        );
        batch.begin(fixture.renderer);
        actor.draw(batch);
        batch.end();
        check(
            fixture.pixel(2, 1).b == 255,
            "Actors must draw the frame selected by their latest update"
        );

        actor.setSprite(walkLeft);
        fixture.clear();
        batch.begin(fixture.renderer);
        actor.draw(batch);
        batch.end();
        check(
            actor.getSprite().getFrameIndex() == 0 && fixture.pixel(2, 1).b == 255,
            "Changing direction must immediately render the replacement's first frame without moving"
        );
        actor.act(0.125f);
        fixture.clear();
        batch.begin(fixture.renderer);
        actor.draw(batch);
        batch.end();
        check(fixture.pixel(2, 1).r == 255, "Actors must continue animating after a sprite change");

        kit2d::Actor empty;
        batch.begin(fixture.renderer);
        empty.draw(batch);
        batch.end();
        check(batch.renderCalls == 0, "Actors without sprites must submit no geometry");
    }

}

int main() {
    try {
        testUpdatesAndSwitching();
        testDrawing();
    } catch (const std::exception& error) {
        std::cerr << error.what() << '\n';
        return 1;
    }
    return 0;
}
