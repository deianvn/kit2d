#ifndef KIT2D_SCENE_ACTOR_HPP
#define KIT2D_SCENE_ACTOR_HPP

#include "../core/AssetManager.hpp"
#include "../sprite/SpriteBatch.hpp"

namespace kit2d {

    class Actor {
    public:
        Actor() = default;
        explicit Actor(Sprite sprite);
        virtual ~Actor() = default;
        void setSprite(Sprite sprite);
        Sprite& getSprite();
        const Sprite& getSprite() const;
        void setPosition(float x, float y);
        const Vec2& getPosition() const;
        virtual void prepare(AssetManager& assetManager);
        virtual void act(float deltaTime);
        virtual void draw(SpriteBatch& spriteBatch);

    private:
        Sprite sprite;
    };

}

#endif
