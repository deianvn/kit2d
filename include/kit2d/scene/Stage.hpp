#ifndef KIT2D_SCENE_STAGE_HPP
#define KIT2D_SCENE_STAGE_HPP

#include "../core/Renderer.hpp"
#include "../sprite/SpriteBatch.hpp"
#include "Scene.hpp"

namespace kit2d {

  class Scene;

  using SceneRef = std::shared_ptr<Scene>;

  class Stage {
  public:
    Stage();
    virtual ~Stage();
    void render(Renderer& renderer);
    void update(float deltaTime);
    void playScene(SceneRef scene);
  private:
    SceneRef scene { nullptr };
    SpriteBatch spriteBatch {};
  };

}

#endif
