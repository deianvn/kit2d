#include "../include/kit2d/scene.hpp"
#include "PlayScene.hpp"

using namespace kit2d;

void loop(Kit& kit);

int main() {
    Kit { Flags::INIT_VIDEO }.run(loop);
    return 0;
}

void loop(Kit& kit) {
    Rect bounds = kit.getDisplayBounds();
    Window window { "Neon Break - kit2d", bounds.x + (bounds.width - 480) / 2,
        bounds.y + (bounds.height - 800) / 2, 480, 800, Flags::WINDOW_SHOWN };
    kit2d::Stage stage { window };
    stage.setScene(std::make_shared<PlayScene>(stage));
    stage.play();
}
