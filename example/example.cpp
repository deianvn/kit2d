#include <iostream>
#include <string>
#include "../include/kit2d.hpp"

using namespace kit2d;

void run(Kit& kit);

int main() {

  Kit { Flags::INIT_VIDEO }.run([](auto& kit)
  {
    run(kit);
  });

  return 0;
}

void run(Kit& kit) {
  Rect bounds = kit.getDisplayBounds();
  Window window { "My window",
    bounds.x, bounds.y,
    640, 480,
    Flags::WINDOW_OPENGL };
  Texture texture = window.loadTexture("SDL_logo.png");
  window.renderer();
  window.onRender([&texture, &kit](auto& renderer) {
    renderer.setColor(255, 0, 0, 255);
    renderer.clear();
    renderer.drawTexture(texture, 10, 0);
    renderer.present();
    kit.delay(100);
  });
  window.loop();
}
