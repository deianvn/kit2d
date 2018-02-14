#include "../../include/kit2d/scene/Director.hpp"

namespace kit2d {

  Director::Director(Stage& stage) :
    stage(stage),
    assetManager(stage.createAssetManager()) {}

  Director::~Director() {}

  AssetManager& Director::getAssetManager() {
    return assetManager;
  }

  void Director::switchScene(SceneRef scene) {
    stage.playScene(scene);
  }

}
