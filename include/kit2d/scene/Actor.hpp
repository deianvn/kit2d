#ifndef KIT2D_SCENE_ACTOR_HPP
#define KIT2D_SCENE_ACTOR_HPP

#include "../core/AssetManager.hpp"
#include "../sprite/SpriteBatch.hpp"

namespace kit2d {

  class Actor {
  public:
    Actor();
    virtual ~Actor();
    virtual void prepare(AssetManager& assetManager);
    virtual void act(float deltaTime);
    virtual void draw(SpriteBatch spriteBatch);
  private:
  };

}

#endif
