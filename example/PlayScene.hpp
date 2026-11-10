#ifndef KIT2D_EXAMPLE_PLAY_SCENE_HPP
#define KIT2D_EXAMPLE_PLAY_SCENE_HPP

#include "../include/kit2d/scene.hpp"

#include "Board.hpp"

class PlayScene : public kit2d::Scene {
public:
    PlayScene(kit2d::Stage& stage);
    void prepare() override;
    void update(float deltaTime) override;
    void render(kit2d::SpriteBatch& spriteBatch) override;

private:
    Board board;
    int lastMouseX { 0 };
    bool mouseControl { false };
    bool spaceHeld { false };
    bool clickHeld { false };
    bool pauseHeld { false };
    bool restartHeld { false };
    bool quitHeld { false };
};

#endif
