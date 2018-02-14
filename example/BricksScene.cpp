#include "BricksScene.hpp"
#include <iostream>

void BricksScene::prepare() {
  assetManager.loadTexture("SDL_logo.png");
  sprite = kit2d::Sprite(assetManager.getTexture("SDL_logo.png").getRegion());
}

void BricksScene::render(kit2d::SpriteBatch& spriteBatch) {
  spriteBatch.add(sprite);
}

void BricksScene::update(float deltaTime) {

}
