#include "../../include/kit2d/scene/Director.hpp"

namespace kit2d {

  Director::Director(Stage& stage) :
    stage(stage) {}

  Director::~Director() {}

  void Director::switchScene(SceneRef scene) {
    stage.playScene(scene);
  }

}
