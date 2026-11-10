#ifndef KIT2D_EXAMPLE_BRICK_HPP
#define KIT2D_EXAMPLE_BRICK_HPP

#include "../include/kit2d/scene.hpp"

#include <string_view>

class Brick : public kit2d::Actor {
public:
    static constexpr std::string_view types { "SORGCV" };

    Brick(
        float x,
        float y,
        float width,
        float height,
        char type
    );
    void prepare(kit2d::AssetManager& assetManager) override;
    void reset();
    int hit();
    bool isDestroyed() const {
        return hits == 0;
    }
    int getHitsRemaining() const {
        return hits;
    }

private:
    char type;
    int hits;
    std::vector<kit2d::TextureView> frames;
    std::vector<kit2d::TextureView> damagedFrames;
};

#endif
