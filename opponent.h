#include "cpputils/graphics/image.h"
#include "game_element.h"
#ifndef OPPONENT_H
#define OPPONENT_H

class OpponentProjectile : public GameElement {
 public:
  OpponentProjectile() : GameElement(0, 0, 32, 32) {}
  OpponentProjectile(const int &x, const int &y) : GameElement(x, y, 32, 32) {}
  void Draw(graphics::Image &screen) override;
  void Move(const graphics::Image &screen) override;
};
class Opponent : public GameElement {
 public:
  Opponent() : GameElement(0, 0, 32, 32) {}
  Opponent(const int &x, const int &y) : GameElement(x, y, 32, 32) {}
  void Draw(graphics::Image &sceen) override;
  void Move(const graphics::Image &screen) override;
  std::unique_ptr<OpponentProjectile> LaunchProjectile();

 private:
  int numberCount_;
};

#endif
