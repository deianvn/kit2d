#include "SDL2.hpp"
#include "../include/Err.hpp"

namespace kit2d {

  Err::Err() : std::runtime_error(SDL_GetError()) {}

}
