#ifndef KIT2D_SPRITE_VEC2_HPP
#define KIT2D_SPRITE_VEC2_HPP

namespace kit2d {

  class Vec2 {
  public:
    Vec2() : x(0.0f), y(0.0f) {}
    Vec2(float x, float y) : x(x), y(y) {}
    Vec2 add(Vec2& v);
    float x, y;
  };

}

#endif
