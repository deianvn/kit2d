#include "../../include/kit2d/scene/Stage.hpp"
#include "../../include/kit2d/scene/Scene.hpp"

namespace kit2d {

    Stage::Stage(Window& window) : window(window) {
        window.onUpdate([this](float deltaTime) {
            if (this->scene)
                this->scene->update(deltaTime);
        });

        window.onRender([this](Renderer& renderer) {
            if (this->scene) {
                renderer.setColor(0, 0, 0, 255);
                renderer.clear();
                spriteBatch.begin(renderer);
                this->scene->render(spriteBatch);
                spriteBatch.end();
                renderer.present();
            }
        });
    }

    void Stage::setScene(std::shared_ptr<Scene> scene) {
        this->scene = scene;
        scene->prepare();
    }

    void Stage::play() {
        window.loop();
    }

    Rect Stage::bounds() {
        return window.getBounds();
    }

}
