#ifndef KIT2D_CORE_ASSET_MANAGER_HPP
#define KIT2D_CORE_ASSET_MANAGER_HPP

#include <map>
#include <string>
#include "Window.hpp"
#include "Texture.hpp"
#include "Atlas.hpp"

namespace kit2d {

  class AssetManager {
  public:

    using TextureMap = std::map<std::string,
      const Texture>;
    using AtlasMap = std::map<std::string,
      const Atlas>;

    AssetManager(Window& window) : window(window) {};
    virtual ~AssetManager();

    void process();
    bool ready();

    void loadTexture(const char* path);
    void addTexture(const char* id, const Texture texture);
    const Texture& getTexture(const char* id);

    void addAtlas(const char* id, const Atlas atlas);
    void loadAtlas(const char* path);

  private:
    Window& window;
    //TextureMap textureMap;
    //AtlasMap atlasMap;
  };

}

#endif
