#include "../../include/kit2d/core/AssetManager.hpp"
#include <iostream>

namespace kit2d {

    AssetManager::AssetManager() {
    }

    AssetManager::~AssetManager() {
    }

    void AssetManager::loadTexture(const char* path) {
        auto process = [this, p = std::string { path }]() {
            auto result = this->textureMap.insert(std::make_pair(p, Texture {}));
            if (result.second) {
                auto& pair = result.first;
                pair->second.loadFromFile(p.c_str());
            }
        };
        processes.push(process);
    }

    void AssetManager::addTexture(const char* id, Texture&& texture) {
        textureMap.emplace(std::string { id }, std::move(texture));
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
        asyncFuture = std::async(std::launch::async, [this]() { this->process(); });
    }

    bool AssetManager::ready() {
        return !processing && processes.empty();
    }

}
