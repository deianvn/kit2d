#include "../../include/kit2d/core/SDL2.hpp"
#include "../../include/kit2d/core/Err.hpp"

namespace kit2d {

  Err::Err() : std::runtime_error(SDL_GetError()) {}

}
