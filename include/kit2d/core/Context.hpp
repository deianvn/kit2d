#ifndef KIT2D_CORE_CONTEXT_HPP
#define KIT2D_CORE_CONTEXT_HPP

#include "SDL2.hpp"

namespace kit2d {

  class Context {
  public:
    Context(SDL_Renderer* handle) : handle(handle) {}
    virtual ~Context();
    SDL_Renderer* handle;
  };

}

#endif
