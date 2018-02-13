#ifndef KIT2D_EXAMPLE_BRICKS_SCENE_HPP
#define KIT2D_EXAMPLE_BRICKS_SCENE_HPP

#include "../include/kit2d/scene/Scene.hpp"

class BricksScene : public kit2d::Scene {
public:
  BricksScene(kit2d::Director& director) :
    kit2d::Scene(director) {}

  void render(kit2d::SpriteBatch& spriteBatch);
  void update(float deltaTime);
private:
  
};

#endif
