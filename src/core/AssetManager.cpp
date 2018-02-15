#include "../../include/kit2d/core/AssetManager.hpp"
#include <iostream>

namespace kit2d {

  AssetManager::AssetManager() {}

  AssetManager::~AssetManager() {}

  void AssetManager::loadTexture(const char* path) {
    auto result = textureMap.insert(
      std::make_pair(std::string { path }, Texture {}));
    if (result.second) {
      auto& pair = result.first;
      pair->second.loadFromFile(path);
    }
  }

  void AssetManager::addTexture(const char* id, Texture texture) {

  }

  TextureView AssetManager::getTexture(const char* id) {
    return TextureView { textureMap.find(std::string { id })->second };
  }

  void AssetManager::addAtlas(const char* id, Atlas atlas) {

  }

  void AssetManager::loadAtlas(const char* path) {

  }

  void AssetManager::process() {

  }

  bool AssetManager::ready() {
    return false;
  }

}
