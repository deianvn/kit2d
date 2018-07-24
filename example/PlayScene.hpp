#ifndef KIT2D_EXAMPLE_PLAY_SCENE_HPP
#define KIT2D_EXAMPLE_PLAY_SCENE_HPP

#include "../include/kit2d/scene.hpp"

#include "Board.hpp"

class PlayScene : public kit2d::Scene {
public:
  PlayScene(kit2d::Stage& stage);
  void prepare();
private:
  Board board;
};

#endif
