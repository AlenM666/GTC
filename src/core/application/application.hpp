#pragma once

#include "../time/time.hpp"
#include "../window/window.hpp"
#include "../../world/test/test_world.hpp"
#include "../../gameplay/player/player.hpp"

#include <cstdint>


namespace Core 
{

class Application final 
{
public:
  Application( std::int32_t width, std::int32_t height, const char * title);

  ~Application() = default;

  Application(const Application&) = delete;
  Application& operator=(const Application&) = delete;

  Application(Application&&) = delete;
  Application& operator=(Application&&) = delete;

  int run();

private:
  void proces_input();
  void simulate(float fixed_delta);
  void update(float delta);
  void render(float interpolation);


private:
  static constexpr float FIXED_DELTA = 1.0F / 60.0F;
  static constexpr int MAX_FIXED_STEPS = 8;
  MainWindow main_window_;
  Time time_;
  World::TestWorld test_world_;

  bool running_ = true; 

  float physics_accumulator_ = 0.0F;

};





}

