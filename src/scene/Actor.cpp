#include "../../include/kit2d/scene/Actor.hpp"

#include <utility>

namespace kit2d {

    Actor::Actor(Sprite sprite) : sprite(std::move(sprite)) {
        this->sprite.restart();
    }

    void Actor::setSprite(Sprite sprite) {
        sprite.setPosition(this->sprite.position.x, this->sprite.position.y);
        sprite.restart();
        this->sprite = std::move(sprite);
    }

    Sprite& Actor::getSprite() {
        return sprite;
    }

    const Sprite& Actor::getSprite() const {
        return sprite;
    }

    void Actor::setPosition(float x, float y) {
        sprite.setPosition(x, y);
    }

    const Vec2& Actor::getPosition() const {
        return sprite.position;
    }

    void Actor::prepare(AssetManager&) {
    }

    void Actor::act(float deltaTime) {
        sprite.update(deltaTime);
    }

    void Actor::draw(SpriteBatch& spriteBatch) {
        sprite.draw(spriteBatch);
    }

}
