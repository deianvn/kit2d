#include "BricksScene.hpp"
#include "../include/kit2d/scene.hpp"

using namespace kit2d;

void loop(Kit& kit);

int main() {
  Kit { Flags::INIT_VIDEO }.run(loop);
  return 0;
}

void loop(Kit& kit) {
  Rect bounds = kit.getDisplayBounds();
  Window window { "My window",
    bounds.x, bounds.y,
    640, 480,
    Flags::WINDOW_OPENGL };

  Stage stage;
  Director director { stage };
  director.switchScene(std::make_shared<BricksScene>(director));

  window.onRender([&stage](auto& renderer) {
    renderer.setColor(255, 0, 0, 255);
    stage.render(renderer);
  });
  window.onUpdate([&stage](auto deltaTime) {
    stage.update(deltaTime);
  });
  window.loop();
}
