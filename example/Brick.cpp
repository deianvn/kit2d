#include "Brick.hpp"

#include <stdexcept>
#include <string>

Brick::Brick(
    float x,
    float y,
    float width,
    float height,
    char type
)
    : type(type), hits(type == 'S' ? 2 : 1) {
    if (types.find(type) == std::string_view::npos || width <= 0.0f || height <= 0.0f)
        throw std::invalid_argument { "Invalid brick type or dimensions" };
    setPosition(x, y);
    getSprite().setSize(width, height);
}

void Brick::prepare(kit2d::AssetManager& assetManager) {
    const std::string id(1, type);
    frames = { assetManager.getTexture((id + "0").c_str()),
        assetManager.getTexture((id + "1").c_str()) };
    if (type == 'S') {
        damagedFrames = { assetManager.getTexture("D0"), assetManager.getTexture("D1") };
    }
    reset();
}

void Brick::reset() {
    hits = type == 'S' ? 2 : 1;
    getSprite().setTextureViews(frames);
    getSprite().setFrameRate(4.0f);
}

int Brick::hit() {
    if (isDestroyed())
        return 0;
    --hits;
    if (hits != 0) {
        getSprite().setTextureViews(damagedFrames);
        getSprite().setFrameRate(8.0f);
        return 0;
    }
    return type == 'S' ? 100 : 90 - static_cast<int>(types.find(type)) * 10;
}
