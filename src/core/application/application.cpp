#include "application.hpp"
#include "../../core/window/window.hpp"
#include <algorithm>
#include <cstdint>
#include <raylib.h>

namespace Core 
{

Application::Application(
  const std::int32_t width,
  const std::int32_t height,
  const char* const title
) : main_window_(width, height, title) {}

int Application::run() 
{
  time_.reset();
  if (!test_world_.load())
  {
    return -1;
  }

  while (running_ && !main_window_.should_close())
  {
    // timing
    time_.update();

    const float delta = time_.delta_seconds();

    // input
    proces_input();


    // fixed timestep physics
    physics_accumulator_ += delta;



    int fixed_steps = 0;
    while (physics_accumulator_ >= FIXED_DELTA && fixed_steps < MAX_FIXED_STEPS)
    {
      simulate(FIXED_DELTA);
      physics_accumulator_ -= FIXED_DELTA;
      ++fixed_steps;
    }


    // If the game was stalled for too long, throw away the excessive 
    // accumulated simulation time rather than running hundreds of physics 
    // iterations and creating a "spiral of death". 
    if (fixed_steps == MAX_FIXED_STEPS) 
    {
        physics_accumulator_ = 0.0F; 
    }


    // variable-rate gameplay update 
    update(delta);


    // rendering
    const float interpolation = std::clamp(
      physics_accumulator_ / FIXED_DELTA,
      0.0F,
      1.0F
    );
    
    test_world_.update(delta);

    main_window_.begin_frame();
      render(interpolation);
      test_world_.draw();
    main_window_.end_frame();
  }

  running_ = false;

  return 0;
}


void Application::proces_input()
{
  // Application-level input belongs here. 
  // Example: 
  // if (IsKeyPressed(KEY_ESCAPE)) 
  // {  running_ = false; }
  if (IsKeyPressed(KEY_ESCAPE))
  {
    running_ = false;
  }
}

void Application::simulate( const float fixed_delta ) 
{ 

  // put deterministic gameplay/physics here

  // player movement physics
  // collison
  // rigid bodies
  // projectile simulation
  // AI fixed simulation
  // ...



  static_cast<void>(fixed_delta);

}

void Application::update(const float delta)
{
  // variable-rate systems

  // UI animations
  // camera smoothing
  // particles
  // timestepnon-physics gameplay
  // ...

  static_cast<void>(delta);

}

void Application::render(const float interpolation)
{
  static_cast<void>(interpolation);

  // ClearBackground(RAYWHITE);
  ClearBackground(BLACK);

  DrawText( "GTC", 40, 40, 32, BLACK);

  DrawText( "Application running", 40, 90, 20, DARKGRAY);

}

}
