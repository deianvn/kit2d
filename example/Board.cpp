#include "Board.hpp"

#include <iostream>
#include <string>
#include <fstream>

void Board::draw(kit2d::SpriteBatch spriteBatch) {
  for (auto& actor : bricks) {
    actor->draw(spriteBatch);
  }
  ball->draw(spriteBatch);
  player->draw(spriteBatch);
}

void Board::prepare(kit2d::AssetManager& assetManager) {
  std::cout << "Prepared" << std::endl;
}

void Board::load(const char* path) {
  std::ifstream file;
  file.open(path);
  if (file.is_open()) {
    std::string line;
    while (std::getline(file, line)) {
      std::cout << line << std::endl;
    }
  } else {
    throw std::runtime_error { "Can not read level data" };
  }
}

void Board::center(int width, int height) {
  this->x = (width - this->width) / 2;
  this->y = (height - this->height) / 2;
}
