#pragma once

#include <cstdint>

namespace Core {

class MainWindow final
{
public:
  MainWindow(
    std::int32_t width,
    std::int32_t height,
    const char* title
  );

  ~MainWindow();

  // copy constructor
  MainWindow(const MainWindow&) = delete;
  MainWindow& operator=(const MainWindow&) = delete;

  // move constructor
  MainWindow(MainWindow&&) = delete;
  MainWindow& operator=(MainWindow&&) = delete;

  [[nodiscard]]
  bool should_close() const noexcept;

  void begin_frame() const noexcept;
  void end_frame() const noexcept;

  void set_target_fps(std::int32_t fps) const noexcept;

  [[nodiscard]]
  std::int32_t width() const noexcept;

  [[nodiscard]]
  std::int32_t height() const noexcept;

  [[nodiscard]]
  float render_width() const noexcept;

  [[nodiscard]]
  float render_height() const noexcept;
};

}

