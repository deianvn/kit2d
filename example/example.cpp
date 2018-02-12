#include <iostream>
#include <string>
#include "../include/kit2d.hpp"

using namespace kit2d;

int main() {
  Kit kit { Flags::INIT_VIDEO };
  {
    Rect bounds = kit.getDisplayBounds();
    Window window { "My window",
      bounds.x, bounds.y,
      640, 480,
      Flags::WINDOW_OPENGL };
    Texture texture = window.loadTexture("SDL_logo.png");
    window.onRender([&texture](Renderer& renderer) {
      renderer.clear();
      renderer.drawTexture(texture, 0, 0);
      renderer.present();
    });
    window.loop();
  }
  kit.quit();
  return 0;
}
