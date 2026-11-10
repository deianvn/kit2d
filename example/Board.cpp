#include "Board.hpp"

#include <algorithm>
#include <array>
#include <cmath>
#include <fstream>
#include <stdexcept>
#include <utility>

namespace {

    constexpr SDL_Color cyan { 98, 229, 244, 255 };
    constexpr SDL_Color white { 229, 240, 255, 255 };
    constexpr SDL_Color muted { 112, 137, 167, 255 };
    constexpr SDL_Color orange { 255, 183, 99, 255 };

    constexpr std::array<std::array<unsigned char, 5>, 36> glyphs { { { 2, 5, 7, 5, 5 },
        { 6, 5, 6, 5, 6 }, { 3, 4, 4, 4, 3 }, { 6, 5, 5, 5, 6 }, { 7, 4, 6, 4, 7 },
        { 7, 4, 6, 4, 4 }, { 3, 4, 5, 5, 3 }, { 5, 5, 7, 5, 5 }, { 7, 2, 2, 2, 7 },
        { 1, 1, 1, 5, 2 }, { 5, 5, 6, 5, 5 }, { 4, 4, 4, 4, 7 }, { 5, 7, 7, 5, 5 },
        { 5, 7, 7, 7, 5 }, { 2, 5, 5, 5, 2 }, { 6, 5, 6, 4, 4 }, { 2, 5, 5, 3, 1 },
        { 6, 5, 6, 5, 5 }, { 3, 4, 2, 1, 6 }, { 7, 2, 2, 2, 2 }, { 5, 5, 5, 5, 7 },
        { 5, 5, 5, 5, 2 }, { 5, 5, 7, 7, 5 }, { 5, 5, 2, 5, 5 }, { 5, 5, 2, 2, 2 },
        { 7, 1, 2, 4, 7 }, { 7, 5, 5, 5, 7 }, { 2, 6, 2, 2, 7 }, { 6, 1, 2, 4, 7 },
        { 6, 1, 2, 1, 6 }, { 5, 5, 7, 1, 1 }, { 7, 4, 6, 1, 6 }, { 3, 4, 6, 5, 2 },
        { 7, 1, 2, 2, 2 }, { 2, 5, 2, 5, 2 }, { 2, 5, 3, 1, 6 } } };

    kit2d::Texture makeBrickTexture(
        int width,
        int height,
        SDL_Color color,
        bool bright
    ) {
        const float strength = bright ? 1.0f : 0.55f;
        const auto channel = [strength](Uint8 value) {
            return static_cast<Uint8>(value * strength);
        };
        auto texture = kit2d::Texture::createSolid(
            width, height, channel(color.r), channel(color.g), channel(color.b)
        );
        kit2d::Renderer renderer;
        renderer.setTarget(&texture);
        renderer.setColor(color.r, color.g, color.b, 255);
        renderer.fillRect(kit2d::Rect { 2, 2, width - 4, 2 });
        renderer.setColor(225, 240, 255, 255);
        renderer.fillRect(kit2d::Rect { 2, 2, bright ? width - 4 : width / 3, 1 });
        renderer.setColor(channel(color.r) / 2, channel(color.g) / 2, channel(color.b) / 2, 255);
        renderer.fillRect(kit2d::Rect { 0, height - 3, width, 3 });
        renderer.fillRect(kit2d::Rect { width - 2, 0, 2, height });
        renderer.setTarget();
        return texture;
    }

    kit2d::Texture makePaddleTexture() {
        auto texture = kit2d::Texture::createSolid(84, 14, 144, 164, 193);
        kit2d::Renderer renderer;
        renderer.setTarget(&texture);
        renderer.setColor(225, 240, 255, 255);
        renderer.fillRect(kit2d::Rect { 9, 2, 66, 3 });
        renderer.setColor(45, 70, 100, 255);
        renderer.fillRect(kit2d::Rect { 9, 10, 66, 4 });
        renderer.setColor(98, 229, 244, 255);
        renderer.fillRect(kit2d::Rect { 0, 0, 9, 14 });
        renderer.fillRect(kit2d::Rect { 75, 0, 9, 14 });
        renderer.setTarget();
        return texture;
    }

    kit2d::Texture makeBallTexture() {
        auto texture = kit2d::Texture::createSolid(12, 12, 0, 0, 0, 0);
        kit2d::Renderer renderer;
        renderer.setTarget(&texture);
        for (int y = 0; y < 12; ++y) {
            for (int x = 0; x < 12; ++x) {
                const float dx = x - 5.5f, dy = y - 5.5f;
                if (dx * dx + dy * dy > 36.0f)
                    continue;
                const Uint8 shade = x + y < 11 ? 255 : 170;
                renderer.setColor(shade, 240, 255, 255);
                renderer.drawPoint(x, y);
            }
        }
        renderer.setTarget();
        return texture;
    }

}

Board::Board(
    int x,
    int y,
    int width,
    int height
)
    : x(x), y(y), width(width), height(height) {
    if (width < 240 || height < 360)
        throw std::invalid_argument { "The playfield must be at least 240 by 360 pixels" };
    paddle.getSprite().setSize(paddleWidth, paddleHeight);
    ball.getSprite().setSize(ballRadius * 2.0f, ballRadius * 2.0f);
    serve();
}

float Board::left() const {
    return x + 20.0f;
}

float Board::right() const {
    return x + width - 20.0f;
}

float Board::top() const {
    return y + 80.0f;
}

float Board::bottom() const {
    return y + height - 52.0f;
}

void Board::load(const char* path) {
    std::ifstream file(path);
    if (!file)
        throw std::runtime_error { std::string { "Cannot read level: " } + path };

    std::vector<std::string> rows;
    std::size_t columns = 0;
    std::string line;
    while (std::getline(file, line)) {
        if (!line.empty() && line.back() == '\r')
            line.pop_back();
        if (line.empty() || line.front() == '#')
            continue;
        for (char cell : line) {
            if (cell != ' ' && cell != '0' && Brick::types.find(cell) == std::string_view::npos)
                throw std::runtime_error { std::string { "Unknown brick type in level: " } + path };
        }
        columns = std::max(columns, line.size());
        rows.push_back(line);
    }
    if (file.bad())
        throw std::runtime_error { std::string { "Error reading level: " } + path };
    if (columns == 0 || columns > static_cast<std::size_t>((right() - left()) / 12.0f) ||
        rows.size() * 24.0f > bottom() - top() - 180.0f)
        throw std::runtime_error { "The level is empty or does not fit the playfield" };

    const int cellWidth = static_cast<int>((right() - left()) / columns);
    std::vector<Brick> loadedBricks;
    for (std::size_t row = 0; row < rows.size(); ++row) {
        for (std::size_t col = 0; col < rows[row].size(); ++col) {
            const char cell = rows[row][col];
            if (cell == ' ' || cell == '0')
                continue;
            loadedBricks.emplace_back(
                left() + col * cellWidth + 2.0f, top() + 62.0f + row * 24.0f,
                static_cast<float>(cellWidth - 4), 20.0f, cell
            );
        }
    }
    if (loadedBricks.empty())
        throw std::runtime_error { "The level must contain at least one brick" };
    brickWidth = cellWidth - 4;
    bricks = std::move(loadedBricks);
    restart();
}

void Board::prepare(kit2d::AssetManager& assetManager) {
    if (bricks.empty())
        throw std::logic_error { "Load a level before preparing the board" };
    constexpr std::array<SDL_Color, 7> colors { { { 175, 199, 222, 255 }, { 255, 173, 86, 255 },
        { 255, 105, 121, 255 }, { 112, 229, 164, 255 }, { 89, 205, 244, 255 },
        { 184, 141, 245, 255 }, { 255, 115, 202, 255 } } };
    const std::string types = std::string { Brick::types } + 'D';
    for (std::size_t index = 0; index < types.size(); ++index) {
        for (int frame = 0; frame < 2; ++frame) {
            const std::string id = std::string(1, types[index]) + std::to_string(frame);
            assetManager.addTexture(
                id.c_str(), makeBrickTexture(brickWidth, 20, colors[index], frame == 1)
            );
        }
    }
    assetManager.addTexture("pixel", kit2d::Texture::createSolid(1, 1, 255, 255, 255));
    assetManager.addTexture("paddle", makePaddleTexture());
    assetManager.addTexture("ball", makeBallTexture());
    pixel = assetManager.getTexture("pixel");
    paddle.setSprite(kit2d::Sprite { assetManager.getTexture("paddle") });
    ball.setSprite(kit2d::Sprite { assetManager.getTexture("ball") });
    for (auto& brick : bricks)
        brick.prepare(assetManager);
    restart();
}

void Board::setInput(Input input) {
    if (!std::isfinite(input.movement) || !std::isfinite(input.pointerX))
        throw std::invalid_argument { "Paddle input must be finite" };
    input.movement = std::clamp(input.movement, -1.0f, 1.0f);
    this->input = input;
}

void Board::restart() {
    for (auto& brick : bricks)
        brick.reset();
    score = 0;
    lives = 3;
    remainingBricks = bricks.size();
    serve();
}

void Board::serve() {
    state = State::Ready;
    ballSpeed = initialBallSpeed;
    ballVelocity = {};
    paddle.setPosition((left() + right() - paddleWidth) / 2.0f, bottom() - 36.0f);
    attachBall();
}

void Board::attachBall() {
    ballPosition = { paddle.getPosition().x + paddleWidth / 2.0f,
        paddle.getPosition().y - ballRadius - 2.0f };
    ball.setPosition(ballPosition.x - ballRadius, ballPosition.y - ballRadius);
}

void Board::movePaddle(const Input& controls, float deltaTime) {
    float movement = controls.movement * paddleSpeed * deltaTime;
    if (controls.usePointer && controls.movement == 0.0f) {
        movement = std::clamp(
            controls.pointerX - paddleWidth / 2.0f - paddle.getPosition().x,
            -paddleSpeed * deltaTime, paddleSpeed * deltaTime
        );
    }
    paddle.setPosition(
        std::clamp(paddle.getPosition().x + movement, left(), right() - paddleWidth),
        paddle.getPosition().y
    );
}

void Board::act(float deltaTime) {
    if (!std::isfinite(deltaTime) || deltaTime < 0.0f)
        throw std::invalid_argument { "Delta time must be finite and non-negative" };
    const Input controls = input;
    input.launch = input.pause = input.restart = false;
    if (controls.restart || (controls.launch && (state == State::Won || state == State::GameOver)))
        restart();
    if (controls.pause || (controls.launch && state == State::Paused)) {
        if (state == State::Playing)
            state = State::Paused;
        else if (state == State::Paused)
            state = State::Playing;
    }
    if (state == State::Paused)
        return;
    for (auto& brick : bricks) {
        if (!brick.isDestroyed())
            brick.act(deltaTime);
    }
    if (state == State::Won || state == State::GameOver)
        return;

    // Bound catch-up after a stall; small physics steps prevent tunneling through bricks.
    const float elapsed = std::min(deltaTime, 0.1f);
    if (state == State::Ready) {
        movePaddle(controls, elapsed);
        attachBall();
        if (!controls.launch)
            return;
        state = State::Playing;
        ballVelocity = { -0.4f * ballSpeed, -std::sqrt(0.84f) * ballSpeed };
        return;
    }
    const int steps = std::max(1, static_cast<int>(std::ceil(elapsed * 240.0f)));
    const float step = elapsed / steps;
    for (int index = 0; index < steps && state == State::Playing; ++index) {
        movePaddle(controls, step);
        moveBall(step);
    }
    ball.setPosition(ballPosition.x - ballRadius, ballPosition.y - ballRadius);
}

bool Board::collide(Brick& brick) {
    const auto& position = brick.getPosition();
    const auto& sprite = brick.getSprite();
    const float closestX = std::clamp(ballPosition.x, position.x, position.x + sprite.getWidth());
    const float closestY = std::clamp(ballPosition.y, position.y, position.y + sprite.getHeight());
    kit2d::Vec2 normal { ballPosition.x - closestX, ballPosition.y - closestY };
    const float distance = normal.length();
    if (distance >= ballRadius)
        return false;

    if (distance > 0.0001f) {
        normal *= 1.0f / distance;
        ballPosition += normal * (ballRadius - distance + 0.01f);
    } else {
        // A newly overlapping center is pushed back against its incoming direction.
        normal = ballVelocity.normalize() * -1.0f;
        ballPosition += normal * (ballRadius + 0.01f);
    }
    const float approach = ballVelocity.dot(normal);
    if (approach >= 0.0f)
        return false;
    ballVelocity -= normal * (2.0f * approach);
    score += brick.hit();
    if (brick.isDestroyed()) {
        --remainingBricks;
        ballSpeed = std::min(600.0f, ballSpeed + 6.0f);
    }
    ballVelocity = ballVelocity.normalize() * ballSpeed;
    if (std::abs(ballVelocity.y) < ballSpeed * 0.2f) {
        ballVelocity.y = (ballVelocity.y < 0.0f ? -0.2f : 0.2f) * ballSpeed;
        ballVelocity.x = std::copysign(
            std::sqrt(ballSpeed * ballSpeed - ballVelocity.y * ballVelocity.y), ballVelocity.x
        );
    }
    return true;
}

void Board::moveBall(float deltaTime) {
    const auto previous = ballPosition;
    ballPosition += ballVelocity * deltaTime;
    if (ballPosition.x < left() + ballRadius) {
        ballPosition.x = left() + ballRadius;
        ballVelocity.x = std::abs(ballVelocity.x);
    } else if (ballPosition.x > right() - ballRadius) {
        ballPosition.x = right() - ballRadius;
        ballVelocity.x = -std::abs(ballVelocity.x);
    }
    if (ballPosition.y < top() + ballRadius) {
        ballPosition.y = top() + ballRadius;
        ballVelocity.y = std::abs(ballVelocity.y);
    }

    const auto& paddlePosition = paddle.getPosition();
    if (ballVelocity.y > 0.0f && previous.y + ballRadius <= paddlePosition.y &&
        ballPosition.y + ballRadius >= paddlePosition.y &&
        ballPosition.x >= paddlePosition.x - ballRadius &&
        ballPosition.x <= paddlePosition.x + paddleWidth + ballRadius) {
        const float offset = std::clamp(
            (ballPosition.x - paddlePosition.x - paddleWidth / 2.0f) / (paddleWidth / 2.0f), -1.0f,
            1.0f
        );
        const float horizontal = std::sin(offset * 1.13446f);
        const float direction = horizontal == 0.0f ? ballVelocity.x : horizontal;
        ballVelocity.x =
            std::copysign(std::max(std::abs(horizontal), 0.18f) * ballSpeed, direction);
        ballVelocity.y = -std::sqrt(ballSpeed * ballSpeed - ballVelocity.x * ballVelocity.x);
        ballPosition.y = paddlePosition.y - ballRadius - 0.01f;
    }

    for (auto& brick : bricks) {
        if (!brick.isDestroyed() && collide(brick))
            break;
    }
    if (remainingBricks == 0) {
        state = State::Won;
        ballVelocity = {};
    } else if (ballPosition.y - ballRadius > bottom()) {
        --lives;
        if (lives == 0) {
            state = State::GameOver;
            ballVelocity = {};
        } else {
            serve();
        }
    }
}

void Board::drawRect(
    kit2d::SpriteBatch& batch,
    float x,
    float y,
    float width,
    float height,
    SDL_Color color
) {
    batch.setColor(color.r / 255.0f, color.g / 255.0f, color.b / 255.0f, color.a / 255.0f);
    batch.draw(pixel, x, y, width, height);
}

void Board::drawText(
    kit2d::SpriteBatch& batch,
    const std::string& text,
    float x,
    float y,
    float scale,
    SDL_Color color
) {
    for (char character : text) {
        const int index = character >= 'A' && character <= 'Z' ? character - 'A'
            : character >= '0' && character <= '9'             ? character - '0' + 26
                                                               : -1;
        for (int row = 0; row < 5; ++row) {
            for (int col = 0; col < 3; ++col) {
                const bool filled = index >= 0 && (glyphs[index][row] & (4 >> col));
                const bool slash = character == '/' && col == 2 - row / 2;
                if (filled || slash)
                    drawRect(batch, x + col * scale, y + row * scale, scale, scale, color);
            }
        }
        x += 4.0f * scale;
    }
}

void Board::drawCenteredText(
    kit2d::SpriteBatch& batch,
    const std::string& text,
    float y,
    float scale,
    SDL_Color color
) {
    drawText(
        batch, text, x + (width - (text.size() * 4.0f - 1.0f) * scale) / 2.0f, y, scale, color
    );
}

void Board::draw(kit2d::SpriteBatch& batch) {
    drawRect(batch, x, y, width, height, SDL_Color { 10, 16, 29, 255 });
    drawRect(
        batch, left(), top(), right() - left(), bottom() - top(), SDL_Color { 15, 25, 43, 255 }
    );
    for (float row = top() + 16; row < bottom(); row += 24) {
        for (float col = left() + 16; col < right(); col += 24)
            drawRect(batch, col, row, 1, 1, SDL_Color { 39, 57, 78, 255 });
    }
    drawRect(batch, left() - 8, top() - 8, 6, bottom() - top() + 8, muted);
    drawRect(batch, right() + 2, top() - 8, 6, bottom() - top() + 8, muted);
    drawRect(batch, left() - 8, top() - 8, right() - left() + 16, 6, cyan);

    for (const auto& brick : bricks) {
        if (!brick.isDestroyed()) {
            drawRect(
                batch, brick.getPosition().x + 2, brick.getPosition().y + 3,
                brick.getSprite().getWidth(), brick.getSprite().getHeight(),
                SDL_Color { 4, 9, 19, 255 }
            );
        }
    }
    for (auto& brick : bricks) {
        if (!brick.isDestroyed())
            brick.draw(batch);
    }
    drawRect(
        batch, paddle.getPosition().x + 2, paddle.getPosition().y + 4, paddleWidth, paddleHeight,
        SDL_Color { 4, 9, 19, 255 }
    );
    paddle.draw(batch);
    ball.draw(batch);

    drawText(batch, "NEON BREAK", left(), y + 16, 2, cyan);
    drawText(batch, "LEVEL 01", right() - 62, y + 16, 2, muted);
    const std::string points = std::to_string(score);
    drawText(
        batch, "SCORE " + std::string(6 - std::min<std::size_t>(6, points.size()), '0') + points,
        left(), y + 42, 2, white
    );
    drawText(batch, "LIVES " + std::to_string(lives), right() - 54, y + 42, 2, orange);
    drawCenteredText(batch, "MOVE A/D OR ARROWS OR MOUSE", y + height - 34, 2, muted);
    drawCenteredText(batch, "SPACE LAUNCH   P PAUSE   R RESTART", y + height - 18, 2, muted);

    if (state != State::Playing) {
        const float panelY = y + height / 2.0f + 6;
        drawRect(
            batch, left() + 12, panelY - 22, right() - left() - 24, 112,
            SDL_Color { 5, 11, 23, 235 }
        );
        drawRect(batch, left() + 12, panelY - 22, right() - left() - 24, 2, cyan);
        const char* heading = state == State::Ready ? "READY PLAYER"
            : state == State::Paused                ? "PAUSED"
            : state == State::Won                   ? "LEVEL CLEAR"
                                                    : "GAME OVER";
        const char* instruction = state == State::Ready ? "SPACE / CLICK TO LAUNCH"
            : state == State::Paused                    ? "P OR SPACE TO RESUME"
                                                        : "SPACE TO PLAY AGAIN";
        drawCenteredText(batch, heading, panelY, 4, state == State::GameOver ? orange : cyan);
        drawCenteredText(batch, instruction, panelY + 36, 2, white);
        drawCenteredText(batch, "R TO RESTART", panelY + 62, 2, muted);
    }
    batch.setColor(1.0f, 1.0f, 1.0f, 1.0f);
}
