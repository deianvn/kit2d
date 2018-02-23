#ifndef KIT2D_EXAMPLE_BOARD_HPP
#define KIT2D_EXAMPLE_BOARD_HPP

#include "../include/kit2d/scene.hpp"

class Board : public kit2d::Actor {
public:
  void prepare(kit2d::AssetManager& assetManager);
  void draw(kit2d::SpriteBatch spriteBatch);
  void load(const char* path);
  void center(int width, int height);
private:
  int x, y, width, height;
  std::vector<std::shared_ptr<kit2d::Actor>> bricks;
  std::shared_ptr<kit2d::Actor> player;
  std::shared_ptr<kit2d::Actor> ball;
};

#endif
