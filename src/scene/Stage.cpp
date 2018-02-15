#include "../../include/kit2d/scene/Stage.hpp"

namespace kit2d {

  Stage::~Stage() {

  }

  void Stage::playScene(SceneRef scene) {
    this->scene = scene;
    scene->prepare();
  }

  void Stage::play() {
    window.onRender([this](auto& renderer) { this->render(renderer); });
    window.onUpdate([this](float deltaTime) { this->update(deltaTime); });
    window.loop();
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

}
