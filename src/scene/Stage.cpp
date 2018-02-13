#include "../../include/kit2d/scene/Stage.hpp"

namespace kit2d {

  Stage::Stage() {

  }

  Stage::~Stage() {

  }

  void Stage::render(Renderer& renderer) {
    if (scene != nullptr) {
      renderer.clear();
      scene->render(spriteBatch);
      spriteBatch.process(renderer);
    }
  }

  void Stage::update(float deltaTime) {
    if (scene != nullptr) {
      scene->update(deltaTime);
    }
  }

  void Stage::playScene(SceneRef scene) {
    this->scene = scene;
    scene->prepare();
  }

}
