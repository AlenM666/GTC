#include "window.hpp"

#include "raylib.h"
#include <cstdint>
#include <stdexcept>



namespace Core {

MainWindow::MainWindow(
  const std::int32_t width,
  const std::int32_t height,
  const char* title
)
{
  if (width <= 0 || height <= 0) 
  {
    throw std::invalid_argument( "Window dimensions must be grater than zero." );
  }

  if (title == nullptr)
  {
    throw std::invalid_argument( "Window title cannot be null." );
  }



  // Config the window before init
  SetConfigFlags(
    FLAG_WINDOW_RESIZABLE |
    FLAG_WINDOW_HIGHDPI
  );

  InitWindow( width, height, title );

  if (!IsWindowReady())
  {
    throw std::runtime_error( "Failed to initilize game window!" );
  }

  // vsync 
  SetWindowState(FLAG_VSYNC_HINT);

  // fallback frame-rate limit
  SetTargetFPS(144);
}

MainWindow::~MainWindow()
{
  if (IsWindowReady())
  {
    CloseWindow();
  }
}


bool MainWindow::should_close() const noexcept
{
  return WindowShouldClose();
}

void MainWindow::begin_frame() const noexcept
{
  BeginDrawing();
}

void MainWindow::end_frame() const noexcept
{
  EndDrawing();
}

void MainWindow::set_target_fps(const std::int32_t fps) const noexcept
{
  if (fps > 0)
  {
    SetTargetFPS(fps);
  }
}

std::int32_t MainWindow::width() const noexcept
{
  return GetScreenWidth();
}

std::int32_t MainWindow::height() const noexcept 
{
  return GetScreenHeight();
}


float MainWindow::render_width() const noexcept
{
  return static_cast<float>(GetRenderWidth());
}

float MainWindow::render_height() const noexcept
{
  return static_cast<float>(GetRenderHeight());
}

}
