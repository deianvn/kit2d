#ifndef KIT2D_CORE_TYPES_HPP
#define KIT2D_CORE_TYPES_HPP

#include <functional>

#include "Renderer.hpp"

namespace kit2d {

  using OnRenderCallback = std::function<void(Renderer& renderer)>;
  using OnUpdateCallback = std::function<void(float deltaTime)>;

}

#endif
