#ifndef KIT2D_SCENE_DIRECTOR_HPP
#define KIT2D_SCENE_DIRECTOR_HPP

#include "Stage.hpp"
#include "Scene.hpp"

namespace kit2d {

  class Stage;
  class Scene;

  class Director {
  public:
    using SceneRef = std::shared_ptr<Scene>;
    using AssetManagerRef = std::shared_ptr<AssetManager>;

    Director(Stage& stage);
    virtual ~Director();
    void switchScene(SceneRef scene);
  private:
    Stage& stage;
    AssetManager assetManager;
  };

}

#endif
