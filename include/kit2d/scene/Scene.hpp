#ifndef KIT2D_SCENE_SCENE_HPP
#define KIT2D_SCENE_SCENE_HPP

#include "../core/AssetManager.hpp"
#include "../sprite/SpriteBatch.hpp"
#include "Director.hpp"

namespace kit2d {

  class Director;

  class Scene {
  public:
    Scene(Director& director);
    virtual ~Scene();
    virtual void render(SpriteBatch& spriteBatch) = 0;
    virtual void update(float deltaTime) = 0;
    virtual void prepare() = 0;
  protected:
    Director& director;
    AssetManager& assetManager;
  };

}

#endif
