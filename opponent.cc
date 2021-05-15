#include "opponent.h"

#include "game_element.h"

void Opponent::Draw(graphics::Image &screen) {
  std::string files = "myOpponent.bmp";
  Helper(screen, files, x_, y_);
}
void OpponentProjectile::Draw(graphics::Image &screen) {
  std::string files = "myOpponentP.bmp";
  Helper(screen, files, x_, y_);
}
std::unique_ptr<OpponentProjectile> Opponent::LaunchProjectile() {
  numberCount_ += 1;
  if (numberCount_ % 5 == 0) {
    std::unique_ptr<OpponentProjectile> myOpponentP =
        std::make_unique<OpponentProjectile>(GetX(), GetY());
    return std::move(myOpponentP);
  } else {
    return nullptr;
  }
}
void Opponent::Move(const graphics::Image &screen) {
  if (!IsOutOfBounds(screen)) {
    SetX(GetX() + 1);
    SetY(GetY() + 1);
  } else {
    check = false;
  }
}
void OpponentProjectile::Move(const graphics::Image &screen) {
  if (!IsOutOfBounds(screen)) {
    SetX(GetX() + 1);
    SetY(GetY() + 1);
  } else {
    check = false;
  }
}
