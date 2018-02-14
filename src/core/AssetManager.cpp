#include "../../include/kit2d/core/AssetManager.hpp"
#include <iostream>

namespace kit2d {

  AssetManager::~AssetManager() {}

  void AssetManager::loadTexture(const char* path) {
    std::cout << 3 << std::endl;
    addTexture(path, window.loadTexture(path));
    std::cout << 4 << std::endl;
  }

  void AssetManager::addTexture(const char* id, const Texture texture) {
    std::cout << 5 << std::endl;
    //textureMap.insert(std::pair<std::string, Texture>(std::string(id), std::move(texture)));
  }

  const Texture& AssetManager::getTexture(const char* id) {
    const std::string key { id };
    //return textureMap.at(key);
    return nullptr;
  }

  void AssetManager::addAtlas(const char* id, const Atlas atlas) {

  }

  void AssetManager::loadAtlas(const char* path) {

  }

  void AssetManager::process() {

  }

  bool AssetManager::ready() {
    return false;
  }

}
