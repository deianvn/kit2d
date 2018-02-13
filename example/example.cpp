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
  TextureRegion textureRegion = texture.getRegion();
  Sprite sprite { textureRegion };
  long frames = 0;
  float elapsed = 0.0f;

  window.onRender([&sprite, &kit](auto& renderer) {
    renderer.setColor(255, 0, 0, 255);
    renderer.clear();
    //renderer.draw(textureRegion, 0, 0);
    //renderer.present();
    SpriteBatch sb {};
    sb.add(sprite);
    sb.process(renderer);
  });
  window.onUpdate([&sprite, &elapsed, &frames](auto deltaTime) {
    sprite.position.x += 10 * deltaTime;
    frames++;
    elapsed += deltaTime;
    int e = static_cast<int>(elapsed);
    if (e > 0) {
      std::cout << frames / e << std::endl;
    }
  });
  window.loop();
}
