#pragma once

#include "../../gameplay/player/player.hpp"

#include "raylib.h"

namespace World {

class TestWorld final
{
public:
  TestWorld();
  ~TestWorld();

  TestWorld(const TestWorld&) = delete;
  TestWorld& operator=(const TestWorld&) = delete;

  TestWorld(TestWorld&&) = delete;
  TestWorld& operator=(TestWorld&&) = delete;

  bool load();
  void update(float delta_time);
  void draw() const;

private:
  void draw_world() const;
  void draw_debug() const;

private:
  static constexpr float ISLAND_WIDTH = 1800.0F;
  static constexpr float ISLAND_HEIGHT = 1200.0F;

  Gameplay::Player player_;

  Camera2D camera_{};
  Rectangle island_boudns_{};
  Vector2 spawn_positions_{};

};




}
