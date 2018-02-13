#include "../../include/kit2d/scene/Director.hpp"

namespace kit2d {

  Director::~Director() {}

  void Director::switchScene(SceneRef scene) {
    stage.playScene(scene);
  }

}
