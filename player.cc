#include "player.h"

#include "game_element.h"
#include "opponent.h"

void Player::Draw(graphics::Image &screen) {
  std::string files = "myPlayer.bmp";
  Helper(screen, files, x_, y_);
}
void PlayerProjectile::Draw(graphics::Image &screen) {
  std::string files = "myPlayerP.bmp";
  Helper(screen, files, x_, y_);
}
void Player::Move(const graphics::Image &screen) {}
void PlayerProjectile::Move(const graphics::Image &screen) {
  if (!IsOutOfBounds(screen)) {
    SetX(GetX() + 1);
    SetY(GetY() + 1);
  } else {
    check = false;
  }
}
