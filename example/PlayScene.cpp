#include "PlayScene.hpp"

PlayScene::PlayScene(kit2d::Stage& stage) :
  Scene(stage), board(0, 0, 0, 0) {}

void PlayScene::prepare() {
  board.load("assets/level.txt");
  board.prepare(assetManager);
}
