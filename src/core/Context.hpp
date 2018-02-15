#ifndef KIT2D_CORE_CONTEXT_HPP
#define KIT2D_CORE_CONTEXT_HPP

#include "../../include/kit2d/core/SDL2.hpp"

namespace kit2d {

  class Context {
  public:
    Context();
    virtual ~Context();
    static SDL_Renderer* handle;
  };

}

#endif
