#pragma once
#include <memory>

#include "GLFW/glfw3.h"
#include "NativeWindow.h"

namespace awawa {
struct glfw {
  GLFWwindow *window;

  Diligent::NativeWindow native_window() const;
  bool should_close() const;
  void poll_events();

  static std::unique_ptr<glfw> create();

  glfw();
  ~glfw() noexcept;
};
} // namespace awawa