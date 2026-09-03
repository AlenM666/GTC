#include "test_world.hpp"
#include "../../gameplay/player/player.hpp"

#include <raylib.h>


namespace World {

TestWorld::TestWorld()
{
  island_boudns_ = {
    0.0F,
    0.0F,
    ISLAND_WIDTH,
    ISLAND_HEIGHT
  };

  spawn_positions_ = {
    ISLAND_WIDTH * 0.5F,
    ISLAND_HEIGHT * 0.5F,
  };

  camera_ = {
    .offset = {
      640.0F,
      360.0F
    },
    .target = spawn_positions_,
    .rotation = 0.0F,
    .zoom = 1.0F
  };
}

bool TestWorld::load()
{
  if (!player_.load("/assets/test_assets/player.png")) { return false; }
  player_.set_position(spawn_positions_);

  camera_.target = player_.position();

  return true;
}

void TestWorld::update(const float delta_time)
{
  player_.update(delta_time);

  // keep player inside islands for test
  const Rectangle player_bounds = player_.bounds();
  Vector2 position = player_.position();

  const float half_width = player_bounds.width * 0.5F;
  const float half_height = player_.bounds().height * 0.5F;

  const float min_x = island_boudns_.x + half_width;
  const float max_x = island_boudns_.x + island_boudns_.width - half_width;

  const float min_y = island_boudns_.y + half_height;
  const float max_y = island_boudns_.y + island_boudns_.height - half_height;

  if (position.x < min_x) { position.x = min_x; }
  if (position.x > max_x) { position.x = max_x; } 
  if (position.y < min_y) { position.y = min_y; }  
  if (position.y > max_y) { position.y = max_y; } 
  player_.set_position(position);



  // camera follows player
  camera_.target = player_.position();

}

void TestWorld::draw() const
{
  BeginMode2D(camera_);
    draw_world();
    player_.draw();
  EndMode2D();

  draw_debug();
}

void TestWorld::draw_world() const
{
  // Ocean background.
  DrawRectangle( -5000, -5000, 10000, 10000, Color{ 70, 150, 190, 255 });


  // Island.
  DrawRectangleRec( island_boudns_, Color{ 220, 190, 110, 255 });



  // inner grass region -------------------------
  const Rectangle grass{
    80.0F,
    80.0F,
    ISLAND_WIDTH - 160.0F,
    ISLAND_HEIGHT - 160.0F
  };
  DrawRectangleRec( grass, Color{ 95, 170, 90, 255 });



  // Simple test structures.
  DrawRectangle( 350, 250, 180, 120, Color{ 130, 90, 60, 255 });
  DrawRectangle( 1250, 300, 220, 150, Color{ 130, 90, 60, 255 });
  DrawCircle( 550, 800, 50.0F, Color{ 45, 120, 55, 255 });
  DrawCircle( 950, 450, 60.0F, Color{ 45, 120, 55, 255 });



  // Harbor.
  DrawRectangle( 700, 0, 400, 120, Color{ 150, 110, 70, 255 });
  DrawRectangle( 760, -120, 280, 150, Color{ 120, 90, 60, 255 });
}

void TestWorld::draw_debug() const
{
  DrawText( "WASD - Move", 20, 20, 24, WHITE);
  DrawText( "GTC - Pirate Prototype", 20, 50, 20, WHITE);
}





}
