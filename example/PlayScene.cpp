#include "PlayScene.hpp"

PlayScene::PlayScene() : Scene() {}

PlayScene::PlayScene(kit2d::Stage& stage) : Scene(stage) {}

void PlayScene::prepare() {
  board.load("assets/level.txt");
  board.prepare(assetManager);
  board.center(480, 800);
}
