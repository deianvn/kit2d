#ifndef KIT2D_VEC2F_HPP
#define KIT2D_VEC2F_HPP

namespace kit2d {

  class Vec2f {
  public:
    Vec2f() : x(0.0f), y(0.0f) {}
    Vec2f(float x, float y) : x(x), y(y) {}
    Vec2f add(Vec2f& v) {
      return Vec2f { x + v.x, y + v.y };
    }
    float x, y;
  };

}

#endif
