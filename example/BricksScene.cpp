#include "BricksScene.hpp"
#include <iostream>

void BricksScene::prepare() {
  std::cout << 1 << std::endl;
  assetManager.loadTexture("SDL_logo.png");
  std::cout << 2 << std::endl;
}

void BricksScene::render(kit2d::SpriteBatch& spriteBatch) {
  kit2d::Sprite sprite { assetManager.getTexture("SDL_logo.png").getRegion() };
  spriteBatch.add(sprite);
}

void BricksScene::update(float deltaTime) {

}
