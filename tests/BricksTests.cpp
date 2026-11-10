#include "../example/Board.hpp"
#include "TestSupport.hpp"

#include <cmath>
#include <iostream>
#include <limits>

namespace {

    using test::check;
    using test::checkThrows;

    constexpr float step = 1.0f / 120.0f;

    void launch(Board& board) {
        Board::Input input;
        input.launch = true;
        board.setInput(input);
        board.act(step);
        board.setInput({});
    }

    void testControlsAndStates() {
        Board board { 0, 0, 480, 800 };
        board.load("assets/level.txt");
        check(
            board.getState() == Board::State::Ready && board.getLives() == 3 &&
                board.getScore() == 0 && board.getRemainingBricks() == 60,
            "The level must start with 60 bricks, three lives, and a waiting ball"
        );
        Board::Input input;
        input.movement = 1.0f;
        board.setInput(input);
        for (int index = 0; index < 120; ++index)
            board.act(step);
        check(
            board.getPaddle().getPosition().x == 376.0f && board.getBallPosition().x == 418.0f,
            "Keyboard movement must stop at the right wall with the ball attached"
        );
        input = {};
        input.usePointer = true;
        input.pointerX = -100.0f;
        board.setInput(input);
        for (int index = 0; index < 120; ++index)
            board.act(step);
        check(
            board.getPaddle().getPosition().x == 20.0f && board.getBallPosition().x == 62.0f,
            "Mouse movement must stop at the left wall with the ball attached"
        );

        const auto initialPosition = board.getBallPosition();
        launch(board);
        check(
            board.getState() == Board::State::Playing && board.getBallVelocity().y < 0.0f,
            "Launching must give the ball an upward velocity"
        );
        board.act(step);
        check(
            board.getBallPosition().y < initialPosition.y, "The launched ball must actually move"
        );
        input = {};
        input.pause = true;
        board.setInput(input);
        board.act(step);
        const auto pausedPosition = board.getBallPosition();
        const auto pausedVelocity = board.getBallVelocity();
        board.setInput({});
        board.act(1.0f);
        check(
            board.getState() == Board::State::Paused && board.getBallPosition() == pausedPosition &&
                board.getBallVelocity() == pausedVelocity,
            "Pausing must freeze the ball without discarding its velocity"
        );
        launch(board);
        check(
            board.getState() == Board::State::Playing && board.getBallPosition() != pausedPosition,
            "Space must resume a paused game"
        );

        for (float invalid : { -1.0f, std::numeric_limits<float>::infinity(),
                 std::numeric_limits<float>::quiet_NaN() }) {
            const auto position = board.getBallPosition();
            checkThrows<std::invalid_argument>([&] { board.act(invalid); });
            check(
                board.getBallPosition() == position, "Invalid delta time must not mutate the game"
            );
        }
        input = {};
        input.pointerX = std::numeric_limits<float>::quiet_NaN();
        checkThrows<std::invalid_argument>([&] { board.setInput(input); });
        checkThrows<std::runtime_error>([&] { board.load("assets/nonexistent-level.txt"); });
        input = {};
        input.restart = true;
        board.setInput(input);
        board.act(step);
        check(
            board.getState() == Board::State::Ready && board.getLives() == 3 &&
                board.getScore() == 0 && board.getRemainingBricks() == 60,
            "Restart must restore the complete level and reset the score and lives"
        );
        check(
            board.getBallPosition() == kit2d::Vec2 { 240.0f, 704.0f },
            "Restart must return the paddle and ball to their starting positions"
        );
    }

    void testBlinkingAndDamage() {
        test::RenderFixture fixture { 480, 800 };
        kit2d::AssetManager assets;
        Board board { 0, 0, 480, 800 };
        board.load("assets/level.txt");
        board.prepare(assets);
        kit2d::SpriteBatch batch;
        const auto& first = board.getBricks().front();
        const int px = static_cast<int>(first.getPosition().x + 10);
        const int py = static_cast<int>(first.getPosition().y + 10);
        const auto draw = [&] {
            fixture.clear();
            batch.begin(fixture.renderer);
            board.draw(batch);
            batch.end();
            return fixture.pixel(px, py);
        };
        const auto dim = draw();
        check(
            first.getSprite().getTextureViews().size() == 2 &&
                first.getSprite().getFrameIndex() == 0,
            "Bricks must use a real two-texture Sprite animation"
        );
        board.act(0.25f);
        const auto bright = draw();
        check(
            first.getSprite().getFrameIndex() == 1 && bright.r > dim.r && bright.g > dim.g,
            "Sprite updates must visibly blink the rendered brick, not just change metadata"
        );
        draw();
        check(
            first.getSprite().getFrameIndex() == 1, "Drawing the board must not advance animations"
        );
        board.act(0.25f);
        const auto repeated = draw();
        check(
            first.getSprite().getFrameIndex() == 0 && repeated.r == dim.r && repeated.g == dim.g,
            "The brick animation must loop back to its dim frame"
        );

        Brick armored { 0, 0, 34, 20, 'S' };
        armored.prepare(assets);
        const auto healthyTexture = armored.getSprite().getTextureViews()[0].sdlTexture;
        check(
            armored.hit() == 0 && armored.getHitsRemaining() == 1 && !armored.isDestroyed(),
            "Armored bricks must survive the first hit"
        );
        check(
            armored.getSprite().getTextureViews()[0].sdlTexture != healthyTexture &&
                armored.getSprite().getFrameRate() == 8.0f,
            "Damage must replace the Sprite texture sequence and make it blink faster"
        );
        check(
            armored.hit() == 100 && armored.isDestroyed() && armored.hit() == 0,
            "Destroying armor must award points exactly once"
        );
        armored.reset();
        check(
            armored.getHitsRemaining() == 2 && armored.getSprite().getFrameRate() == 4.0f &&
                armored.getSprite().getTextureViews()[0].sdlTexture == healthyTexture,
            "Restarting must restore the armor and its original animation"
        );
    }

    void testLosingLives() {
        Board board { 0, 0, 480, 800 };
        board.load("assets/level.txt");
        for (int life = 3; life > 0; --life) {
            launch(board);
            for (int index = 0; index < 12000 && board.getState() == Board::State::Playing;
                ++index) {
                Board::Input input;
                input.usePointer = true;
                input.pointerX = board.getBallPosition().x < 240.0f ? 460.0f : 20.0f;
                board.setInput(input);
                board.act(step);
            }
            check(board.getLives() == life - 1, "Missing the paddle must cost exactly one life");
            check(
                board.getState() == (life == 1 ? Board::State::GameOver : Board::State::Ready),
                "A miss must wait for another launch, or end the game after the last life"
            );
        }
        const auto position = board.getBallPosition();
        board.act(2.0f);
        check(
            board.getLives() == 0 && board.getBallPosition() == position,
            "Game over must stop physics and must not keep subtracting lives"
        );
        launch(board);
        check(
            board.getState() == Board::State::Playing && board.getLives() == 3 &&
                board.getScore() == 0 && board.getRemainingBricks() == 60,
            "Space after game over must start a fresh playable game"
        );
    }

    void testWinningTheLevel(float deltaTime) {
        Board board { 0, 0, 480, 800 };
        board.load("assets/level.txt");
        launch(board);
        bool bouncedOffPaddle = false;
        const int frameLimit = static_cast<int>(1000.0f / deltaTime);
        for (int index = 0; index < frameLimit && board.getState() == Board::State::Playing;
            ++index) {
            Board::Input input;
            input.usePointer = true;
            input.pointerX = board.getBallPosition().x + std::sin(index * deltaTime * 0.7f) * 26.0f;
            board.setInput(input);
            const auto previousVelocity = board.getBallVelocity();
            const auto previousPosition = board.getBallPosition();
            board.act(deltaTime);
            const auto position = board.getBallPosition();
            check(
                position.x >= 26.0f && position.x <= 454.0f && position.y >= 86.0f,
                "The ball must remain inside the side and top walls"
            );
            check(
                board.getBallVelocity().length() <= 600.01f,
                "Ball acceleration must stop at the maximum playable speed"
            );
            if (previousPosition.y > 650.0f && previousVelocity.y > 0.0f &&
                board.getBallVelocity().y < 0.0f)
                bouncedOffPaddle = true;
        }
        check(bouncedOffPaddle, "The paddle must bounce a falling ball back into play");
        if (board.getState() != Board::State::Won) {
            std::cerr << "Auto-play stopped with " << board.getRemainingBricks() << " bricks, "
                      << board.getLives() << " lives, score " << board.getScore() << '\n';
        }
        check(
            board.getState() == Board::State::Won && board.getRemainingBricks() == 0,
            "Actual ball/paddle/brick collisions must allow the entire level to be completed"
        );
        check(
            board.getScore() == 3990 && board.getLives() == 3,
            "Completing the level must award every brick once without losing lives"
        );
        launch(board);
        check(
            board.getState() == Board::State::Playing && board.getRemainingBricks() == 60 &&
                board.getScore() == 0,
            "A cleared level must be replayable without recreating the scene"
        );
    }

}

int main() {
    try {
        testControlsAndStates();
        testBlinkingAndDamage();
        testLosingLives();
        testWinningTheLevel(1.0f / 30.0f);
        testWinningTheLevel(1.0f / 60.0f);
        testWinningTheLevel(1.0f / 120.0f);
        std::cout << "Bricks gameplay tests passed\n";
        return 0;
    } catch (const std::exception& error) {
        std::cerr << error.what() << '\n';
        return 1;
    }
}
