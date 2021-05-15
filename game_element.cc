#include "game_element.h"

#include "cpputils/graphics/image.h"

void GameElement::Helper(graphics::Image &screen, std::string &object, int &x_,
                         int &y_) {
  graphics::Image mySketch;
  mySketch.Load(object);
  for (int i = 0; i < mySketch.GetWidth(); i++) {
    for (int j = 0; j < mySketch.GetHeight(); j++) {
      screen.SetColor(i + x_, j + y_, mySketch.GetColor(i, j));
    }
  }
}
bool GameElement::IsOutOfBounds(const graphics::Image &screen) {
  if (x_ < 0 || y_ < 0 || x_ > screen.GetWidth() || y_ > screen.GetHeight()) {
    return true;
  } else {
    return false;
  }
}
