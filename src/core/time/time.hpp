#pragma once


namespace Core 
{

class Time final 
{
public:
  Time() noexcept = default;

  void reset() noexcept;

  void update() noexcept;

  [[nodiscard]]
  float delta_seconds() const noexcept;

  [[nodiscard]]
  float unscaled_delta_seconds() const noexcept;

  [[nodiscard]]
  float interpolation_alpha() const noexcept;

  [[nodiscard]]
  double total_seconds() const noexcept;

  [[nodiscard]]
  double frame_count() const noexcept;

  void set_time_scale(float scale) noexcept;

private:
  static constexpr float MAX_FRAME_DELTA = 0.25F;
  static constexpr float FIXED_DELTA = 1.0F / 60.0F;

  float delta_seconds_ = 0.0F;
  float unscaled_delta_seconds_ = 0.0F;

  float accumulator_ = 0.0F;

  float time_scale_ = 1.0F;
  
  double total_seconds_ = 0.0;
  double frame_count_ = 0.0;

};


}
