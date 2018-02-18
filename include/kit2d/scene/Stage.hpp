#ifndef KIT2D_SCENE_STAGE_HPP
#define KIT2D_SCENE_STAGE_HPP

#include "../core/Window.hpp"
#include "Scene.hpp"

namespace kit2d {

  class Stage {
  public:
    Stage(Window& window);
    void setScene(std::shared_ptr<Scene> scene);
    void play();
  private:
    Window& window;
    SpriteBatch spriteBatch {};
    std::shared_ptr<Scene> scene { nullptr };
  };

}

#endif
