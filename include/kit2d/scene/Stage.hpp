#ifndef KIT2D_SCENE_STAGE_HPP
#define KIT2D_SCENE_STAGE_HPP

#include "../core/Window.hpp"
#include "../core/Renderer.hpp"
#include "../sprite/SpriteBatch.hpp"
#include "Scene.hpp"

namespace kit2d {

  class Scene;

  using SceneRef = std::shared_ptr<Scene>;

  class Stage {
  public:
    Stage(Window& window) : window(window) {}
    virtual ~Stage();
    void play();
    void playScene(SceneRef scene);
  private:
    void render(Renderer& renderer);
    void update(float deltaTime);

    Window& window;
    SceneRef scene { nullptr };
    SpriteBatch spriteBatch {};
  };

}

#endif
