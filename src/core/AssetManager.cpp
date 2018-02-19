#include "../../include/kit2d/core/AssetManager.hpp"
#include <iostream>

namespace kit2d {

  AssetManager::AssetManager() {}

  AssetManager::~AssetManager() {}

  void AssetManager::loadTexture(const char* path) {
    auto process = [this, &path]() {
      auto result = this->textureMap.insert(
        std::make_pair(std::string { path }, Texture {}));
      if (result.second) {
        auto& pair = result.first;
        pair->second.loadFromFile(path);
      }
    };
    processes.push(process);
  }

  void AssetManager::addTexture(const char* id, Texture texture) {
    textureMap.insert(
      std::make_pair(std::string { id }, texture));
  }

  TextureView AssetManager::getTexture(const char* id) {
    return TextureView { textureMap.find(std::string { id })->second };
  }

  void AssetManager::addAtlas(const char* id, Atlas atlas) {

  }

  void AssetManager::loadAtlas(const char* path) {

  }

  void AssetManager::process() {
    processing = true;
    while (!processes.empty()) {
      auto process = processes.front();
      process();
      processes.pop();
    }
    processing = false;
  }

  void AssetManager::processAsync() {
    processing = true;
    std::async([this]() { this->process(); });
  }

  bool AssetManager::ready() {
    return false;
  }

}
