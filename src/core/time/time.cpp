#include "time.hpp"
#include "raylib.h"

#include <algorithm>

namespace Core 
{

void Time::reset() noexcept
{
  delta_seconds_ = 0.0F;
  unscaled_delta_seconds_ = 0.0F;
  accumulator_ = 0.0F;
  time_scale_ = 1.0F;
  total_seconds_ = 0.0;
  frame_count_ = 0.0;
}

void Time::update() noexcept
{
  const float raw_delta = GetFrameTime();

  // Prevent a debugger break / window stall / OS hitch from producing
  // an enormous simulation step.
  unscaled_delta_seconds_ = std::clamp(
    raw_delta,
    0.0F,
    MAX_FRAME_DELTA
  );

  delta_seconds_ = unscaled_delta_seconds_ * time_scale_;

  accumulator_ += delta_seconds_;

  total_seconds_ += static_cast<double>(delta_seconds_);

  frame_count_ += 1.0;
}

float Time::delta_seconds() const noexcept
{
  return delta_seconds_;
}

float Time::unscaled_delta_seconds() const noexcept
{
  return unscaled_delta_seconds_;
}

float Time::interpolation_alpha() const noexcept
{
  return accumulator_ / FIXED_DELTA;
}

double Time::total_seconds() const noexcept
{
  return total_seconds_;
}

double Time::frame_count() const noexcept
{
  return frame_count_;
}

void Time::set_time_scale(const float scale) noexcept
{
  time_scale_ = std::max( scale, 0.0F);
}


}
