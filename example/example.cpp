#include <iostream>
#include <string>
#include "../include/kit2d.hpp"

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
  Texture texture = window.loadTexture("SDL_logo.png");
  window.onRender([&texture, &kit](auto& renderer) {
    renderer.setColor(255, 0, 0, 255);
    renderer.clear();
    renderer.drawTexture(texture, 10, 0);
    renderer.present();
  });
  window.loop();
}
