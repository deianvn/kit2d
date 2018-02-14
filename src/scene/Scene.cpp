#include "../../include/kit2d/scene/Scene.hpp"

namespace kit2d {

  Scene::Scene(Director& director) :
    director(director), assetManager(director.getAssetManager()) {}

  Scene::~Scene() {}

}
