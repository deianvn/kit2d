#ifndef KIT2D_SCENE_ACTOR_HPP
#define KIT2D_SCENE_ACTOR_HPP

#include "../sprite/SpriteBatch.hpp"

namespace kit2d {

  class Actor {
  public:
    Actor();
    virtual ~Actor();
    virtual void update(float deltaTime);
    virtual void draw(SpriteBatch spriteBatch);
  private:
  };

}

#endif
