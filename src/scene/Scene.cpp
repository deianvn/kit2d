#include "../../include/kit2d/scene/Scene.hpp"
#include "../../include/kit2d/scene/Stage.hpp"

namespace kit2d {

  Scene::Scene() {}

  Scene::Scene(Stage& stage) : stage(&stage) {}

  Scene::~Scene() {}

  void Scene::setStage(Stage& stage) {
    this->stage = &stage;
  }

  Stage& Scene::getStage() {
    return *stage;
  }

  void Scene::prepare() {}

  void Scene::update(float deltaTime) {}

  void Scene::render(SpriteBatch& spriteBatch) {}

}
