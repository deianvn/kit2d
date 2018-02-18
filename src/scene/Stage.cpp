#include "../../include/kit2d/scene/Stage.hpp"
#include "../../include/kit2d/scene/Scene.hpp"

namespace kit2d {

  Stage::Stage(Window& window) : window(window) {
    window.onUpdate([this](float deltaTime) {
      this->scene->update(deltaTime);
    });

    window.onRender([this](Renderer& renderer) {
      this->scene->render(spriteBatch);
      spriteBatch.process(renderer);
    });
  }

  void Stage::setScene(std::shared_ptr<Scene> scene) {
    this->scene = scene;
  }

  void Stage::play() {
    window.loop();
  }

}
