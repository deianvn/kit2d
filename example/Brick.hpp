#ifndef KIT2D_EXAMPLE_BRICK_HPP
#define KIT2D_EXAMPLE_BRICK_HPP

class Brick : public Actor {
public:
  Brick();
  ~Brick();
  void setSprite(Sprite sprite);
private:
  Sprite sprite {};
};

#endif
