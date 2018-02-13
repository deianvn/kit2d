#ifndef KIT2D_SCENE_DIRECTOR_HPP
#define KIT2D_SCENE_DIRECTOR_HPP

#include "Stage.hpp"

namespace kit2d {

  class Stage;
  class Scene;

  class Director {
  public:
    Director(Stage& stage) : stage(stage) {};
    virtual ~Director();
    void switchScene(SceneRef scene);
  private:
    Stage& stage;
  };

}

#endif
