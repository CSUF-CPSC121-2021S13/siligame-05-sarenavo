#include <iostream>
#include <memory>

#include "cpputils/graphics/image_event.h"
#include "game_element.h"
#include "opponent.h"
#include "player.h"

#ifndef GAME_H
#define GAME_H

class Game : public graphics::AnimationEventListener,
             public graphics::MouseEventListener {
 public:
  Game() : Game(800, 600) {}
  Game(const int &width, const int &height) {
    gamescreen.Initialize(width, height);
  }

  void CreateOpponents() {
    std::unique_ptr<Opponent> abg =
      std::make_unique<Opponent>(gamescreen.GetWidth() * 0.5, 0);
    myOpponent.push_back(std::move(abg));
  }
  void Init() {
    gamescreen.AddMouseEventListener(*this);
    gamescreen.AddAnimationEventListener(*this);
    CreateOpponents();
    myPlayer_.SetX(250);
    myPlayer_.SetY(250);
  }
  void MoveGameElements() {
    for (int j = 0; j < myOpponent.size(); j++) {
      if (myOpponent[j]->GetIsActive()) {
        myOpponent[j]->Move(gamescreen);
      }
    }
    for (int k = 0; k < myOpponentP_.size(); k++) {
      if (myOpponentP_[k]->GetIsActive()) {
        myOpponentP_[k]->Move(gamescreen);
      }
    }
    for (int l = 0; l < myPlayerP_.size(); l++) {
      if (myPlayerP_[l]->GetIsActive()) {
        myPlayerP_[l]->Move(gamescreen);
      }
    }
  }
  void FilterIntersections() {
    for (int j = 0; j < myOpponent.size(); j++) {
      if (myOpponent[j]->IntersectsWith(&myPlayer_)) {
        myOpponent[j]->SetIsActive(false);
        myPlayer_.SetIsActive(false);
        loser_ = true;
      }
    }
    for (int k = 0; k < myOpponentP_.size(); k++) {
      if (myOpponentP_[k]->IntersectsWith(&myPlayer_)) {
        myOpponentP_[k]->SetIsActive(false);
        myPlayer_.SetIsActive(false);
        loser_ = true;
      }
    }
    for (int l = 0; l < myPlayerP_.size(); l++) {
      for (int m = 0; m < myOpponent.size(); m++) {
        if (myPlayerP_[l]->IntersectsWith(myOpponent[m].get())) {
          myPlayerP_[l]->SetIsActive(false);
          myOpponent[m]->SetIsActive(false);
          if (myPlayer_.GetIsActive() == true) {
            winning_points_ = winning_points_ + 1;
          }
        }
      }
    }
  }

  void UpdateScreen() {
    std::string winning_score_ = "Points: ";
    winning_score_ += winning_points_;
    gamescreen.DrawRectangle(0, 0, gamescreen.GetWidth(),
                             gamescreen.GetHeight(), 255, 255, 255);
    gamescreen.DrawText(0, 570, winning_score_, 25, 190, 20, 255);
    if (loser_ == true) {
      gamescreen.DrawText(gamescreen.GetWidth() * 0, gamescreen.GetHeight() * 0,
                          "Lol you lost.", 50, 190, 20, 225);
    }
    for (int j = 0; j < myOpponent.size(); j++) {
      if (myOpponent[j]->GetIsActive()) {
        myOpponent[j]->Draw(gamescreen);
      }
    }
    for (int k = 0; k < myOpponentP_.size(); k++) {
      if (myOpponentP_[k]->GetIsActive()) {
        myOpponentP_[k]->Draw(gamescreen);
      }
    }
    for (int l = 0; l < myPlayerP_.size(); l++) {
      if (myPlayerP_[l]->GetIsActive()) {
        myPlayerP_[l]->Draw(gamescreen);
      }
    }
    if (myPlayer_.GetIsActive() == true) {
      myPlayer_.Draw(gamescreen);
    } else if (myPlayer_.GetIsActive() == false) {
      RemoveInactive();
    }
  }
  void OnMouseEvent(const graphics::MouseEvent &event) override {
    if (event.GetX() > 0 && event.GetX() < gamescreen.GetWidth() &&
        event.GetY() > 0 && event.GetY() < gamescreen.GetHeight()) {
      myPlayer_.SetX(event.GetX() - (myPlayer_.GetWidth() * 0.5));
      myPlayer_.SetY(event.GetY() - (myPlayer_.GetHeight() * 0.5));
    }
    if (event.GetMouseAction() == graphics::MouseAction::kPressed ||
        event.GetMouseAction() == graphics::MouseAction::kDragged) {
      std::unique_ptr<PlayerProjectile> boba =
          std::make_unique<PlayerProjectile>(myPlayer_.GetX(), myPlayer_.GetY());
      myPlayerP_.push_back(std::move(boba));
    }
  }
  void OnAnimationStep() override {
    if (myOpponent.size() == 0) {
      CreateOpponents();
    }
    MoveGameElements();
    FilterIntersections();
    UpdateScreen();
    gamescreen.Flush();
    RemoveInactive();
    LaunchProjectiles();
  }
  int GetScore() const { return winning_points_; }
  bool HasLost() const { return loser_; }
  void LaunchProjectiles() {
    for (int i = 0; i < myOpponent.size(); i++) {
      if (myOpponent[i]->LaunchProjectile() != nullptr) {
        std::unique_ptr<OpponentProjectile> falsies =
            std::make_unique<OpponentProjectile>(myOpponent[i]->GetX(),
                                                 myOpponent[i]->GetY());
        myOpponentP_.push_back(std::move(falsies));
      }
    }
  }
  void RemoveInactive() {
    for (int i = 0; i < myOpponent.size(); i++) {
      if (myOpponent[i]->GetIsActive() == false) {
        myOpponent.erase(myOpponent.begin());
        i = i - 1;
      }
    }
    for (int j = 0; j < myOpponentP_.size(); j++) {
      if (myOpponentP_[j]->GetIsActive() == false) {
        myOpponentP_.erase(myOpponentP_.begin());
        j = j - 1;
      }
    }
    for (int k = 0; k < myPlayerP_.size(); k++) {
      if (myPlayerP_[k]->GetIsActive() == false) {
        myPlayerP_.erase(myPlayerP_.begin());
        k = k - 1;
      }
    }
  }
  graphics::Image &GetGameScreen() { return gamescreen; }
  std::vector<std::unique_ptr<Opponent>> &GetOpponents() { return myOpponent; }
  std::vector<std::unique_ptr<OpponentProjectile>> &GetOpponentProjectiles() {
    return myOpponentP_;
  }
  std::vector<std::unique_ptr<PlayerProjectile>> &GetPlayerProjectiles() {
    return myPlayerP_;
  }
  Player &GetPlayer() { return myPlayer_; }
  void Start() { gamescreen.ShowUntilClosed(); }

 private:
  int winning_points_ = 0;
  bool loser_ = false;
  Player myPlayer_;
  graphics::Image gamescreen;
  std::vector<std::unique_ptr<Opponent>> myOpponent;
  std::vector<std::unique_ptr<OpponentProjectile>> myOpponentP_;
  std::vector<std::unique_ptr<PlayerProjectile>> myPlayerP_;
};

#endif
