#ifndef KIT2D_EXAMPLE_BOARD_HPP
#define KIT2D_EXAMPLE_BOARD_HPP

#include "../include/kit2d/scene.hpp"
#include "Brick.hpp"

#include <string>
#include <vector>

class Board : public kit2d::Actor {
public:
    enum class State { Ready, Playing, Paused, Won, GameOver };

    struct Input {
        float movement { 0.0f };
        bool usePointer { false };
        float pointerX { 0.0f };
        bool launch { false };
        bool pause { false };
        bool restart { false };
    };

    Board(
        int x,
        int y,
        int width,
        int height
    );
    void prepare(kit2d::AssetManager& assetManager) override;
    void act(float deltaTime) override;
    void draw(kit2d::SpriteBatch& spriteBatch) override;
    void load(const char* path);
    void setInput(Input input);
    State getState() const {
        return state;
    }
    int getScore() const {
        return score;
    }
    int getLives() const {
        return lives;
    }
    std::size_t getRemainingBricks() const {
        return remainingBricks;
    }
    const std::vector<Brick>& getBricks() const {
        return bricks;
    }
    const kit2d::Vec2& getBallPosition() const {
        return ballPosition;
    }
    const kit2d::Vec2& getBallVelocity() const {
        return ballVelocity;
    }
    const kit2d::Actor& getPaddle() const {
        return paddle;
    }

private:
    static constexpr float ballRadius = 6.0f;
    static constexpr float paddleWidth = 84.0f;
    static constexpr float paddleHeight = 14.0f;
    static constexpr float paddleSpeed = 620.0f;
    static constexpr float initialBallSpeed = 340.0f;

    float left() const;
    float right() const;
    float top() const;
    float bottom() const;
    void restart();
    void serve();
    void attachBall();
    void movePaddle(const Input& controls, float deltaTime);
    void moveBall(float deltaTime);
    bool collide(Brick& brick);
    void drawRect(
        kit2d::SpriteBatch& batch,
        float x,
        float y,
        float width,
        float height,
        SDL_Color color
    );
    void drawText(
        kit2d::SpriteBatch& batch,
        const std::string& text,
        float x,
        float y,
        float scale,
        SDL_Color color
    );
    void drawCenteredText(
        kit2d::SpriteBatch& batch,
        const std::string& text,
        float y,
        float scale,
        SDL_Color color
    );

    int x, y, width, height;
    int brickWidth { 0 };
    std::vector<Brick> bricks;
    kit2d::Actor paddle;
    kit2d::Actor ball;
    kit2d::TextureView pixel;
    kit2d::Vec2 ballPosition;
    kit2d::Vec2 ballVelocity;
    float ballSpeed { initialBallSpeed };
    State state { State::Ready };
    Input input;
    int score { 0 };
    int lives { 3 };
    std::size_t remainingBricks { 0 };
};

#endif
