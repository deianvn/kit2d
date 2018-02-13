#include "../../include/kit2d/sprite/Vec2.hpp"

namespace kit2d {

  Vec2 Vec2::add(Vec2& v) {
    return Vec2 { x + v.x, y + v.y };
  }

}
