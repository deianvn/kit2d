#ifndef KIT2D_SCENE_SCENE_HPP
#define KIT2D_SCENE_SCENE_HPP

#include "../sprite/SpriteBatch.hpp"

namespace kit2d {

  class Director;

  class Scene {
  public:
    Scene(Director& director) : director(director) {}
    virtual ~Scene();
    virtual void prepare();
    virtual void render(SpriteBatch& spriteBatch) = 0;
    virtual void update(float deltaTime) = 0;
  protected:
    Director& director;
  };

}

#endif
