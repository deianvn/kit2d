#include <iostream>
#include <string>
#include "../include/kit2d.hpp"

using namespace kit2d;

int main() {
  Kit kit { Flags::INIT_VIDEO };
  {
    int dislayCount = kit.countDisplays();
    std::cout
      << "Display count: " << dislayCount << "\n"
      << "Choose display > ";
    int displayIndex;
    std::cin >> displayIndex;
    kit.useDisplay(displayIndex);
    Rect bounds = kit.getDisplayBounds();

    Window window { "My window",
      bounds.x, bounds.y,
      640, 480,
      Flags::WINDOW_OPENGL };
    //Renderer renderer = window.renderer();
    Texture texture = window.loadTexture("SDL_logo.png");

    window.setOnRenderCallback([&texture](Renderer& renderer) {
      renderer.clear();
      renderer.drawTexture(texture);
      renderer.present();
    });

    window.loop();
  }
  Kit::DEFAULT->quit();
  return 0;
}
