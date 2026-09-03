#pragma once

#include "raylib.h"


namespace Gameplay {

class Player final 
{

public:
  Player();
  ~Player();

  Player(const Player&) = delete;
  Player& operator=(const Player&) = delete;

  Player(Player&&) = delete;
  Player& operator=(Player&&) = delete;

  bool load(const char* texture_path);

  void update(float delta_time);

  void draw() const;

  void set_position(Vector2 position) noexcept;

  [[nodiscard]]
  Vector2 position() const noexcept;

  [[nodiscard]]
  Rectangle bounds() const noexcept;

private:
  void handle_movement(float delta_time);

private:
  Texture2D texture_{};
  Vector2 position_ {
    0.0F,
    0.0F
  };

  float move_speed_ = 180.0F;
  float scale_ = 2.0F;
  bool loaded_ = false;
  
};

}
