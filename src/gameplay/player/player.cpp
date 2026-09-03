#include "player.hpp"

#include "raylib.h"
#include <cmath>
#include <stdexcept>

namespace Gameplay {

Player::Player() = default;

Player::~Player() 
{
  if (loaded_)
  {
    UnloadTexture(texture_);
  }
}


bool Player::load(const char* const texture_path)
{
  if(texture_path == nullptr)
  {
    throw std::invalid_argument(
      "Player texture path cannot be null"
    );
  }

  texture_ = LoadTexture(texture_path);
  if (!IsTextureValid(texture_))
  {
    return false;
  }

  loaded_ = true;
  return true;
}

void Player::update(const float delta_time)
{
  handle_movement(delta_time);
}

void Player::handle_movement(const float delta_time)
{
  Vector2 direction {};
  if (IsKeyDown(KEY_W)) { direction.y += 1.0F; }
  if (IsKeyDown(KEY_S)) { direction.y -= 1.0F; }
  if (IsKeyDown(KEY_A)) { direction.x -= 1.0F; }
  if (IsKeyDown(KEY_D)) { direction.x += 1.0F; }

  const float length_squared = (direction.x * direction.x) + (direction.y * direction.y);

  // Normalize diagonal movement so w+d isnt faster than w its a common bug
  if (length_squared > 0.0F)
  {
    const float inverse_length = 1.0F / std::sqrt(length_squared);

    direction.x *= inverse_length;
    direction.y *= inverse_length;
  }

  position_.x += direction.x * move_speed_ * delta_time;
  position_.y += direction.y * move_speed_ * delta_time;

}


void Player::draw() const 
{
  if (!loaded_)
  {
    return;
  }

  const float width = static_cast<float>(texture_.width);
  const float height = static_cast<float>(texture_.height);

  DrawTextureEx(texture_, {position_.x - (width * 0.5F), position_.y - (height * 0.5F)}, 0.0F, scale_, WHITE);
}


void Player::set_position(const Vector2 position) noexcept
{
  position_ = position;
}

Vector2 Player::position() const noexcept
{
  return position_;
}

Rectangle Player::bounds() const noexcept
{
  if (!loaded_)
  {
    return {
      position_.x,
      position_.y,
      0.0F,
      0.0F
    };
  }

  const float width = static_cast<float>(texture_.width) * scale_;
  const float height = static_cast<float>(texture_.height) * scale_;

  return {
    position_.x - (width * 0.5F), 
    position_.y - (height * 0.5F),
    width, 
    height
  };
}

}


