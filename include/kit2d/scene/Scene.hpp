#ifndef KIT2D_SCENE_SCENE_HPP
#define KIT2D_SCENE_SCENE_HPP

#include "../core/AssetManager.hpp"
#include "../sprite/SpriteBatch.hpp"

namespace kit2d {

  class Stage;

  class Scene {
  public:
    Scene();
    Scene(Stage& stage);
    virtual ~Scene();
    void setStage(Stage& stage);
    Stage& getStage();
    virtual void prepare();
    virtual void update(float deltaTime);
    virtual void render(SpriteBatch& spriteBatch);
  private:
    Stage* stage;
    AssetManager assetManager;
  };

}

#endif
