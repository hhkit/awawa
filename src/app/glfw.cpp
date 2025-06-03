#include "app/glfw.h"
#include "GLFW/glfw3.h"
#include <cassert>
#if PLATFORM_MACOS
#define GLFW_EXPOSE_NATIVE_COCOA
extern void *GetNSWindowView(GLFWwindow *wnd);
#endif
#if PLATFORM_WIN32
#define GLFW_EXPOSE_NATIVE_WIN32
#endif
#include "GLFW/glfw3native.h"

static int glfw_counter = 0;

namespace awawa {

Diligent::NativeWindow glfw::native_window() const {
  void *window_handle = NULL;
#if PLATFORM_WIN32
  window_handle = static_cast<void *>(glfwGetWin32Window(window));
#elif PLATFORM_MACOS
  window_handle = static_cast<void *>(GetNSWindowView(window));
#elif PLATFORM_LINUX
  // FIXME: Get x11 or wayland window handle using glfw
  glfwMakeContextCurrent(window);
  throw std::runtime_error("Missing window handle");
#endif

  return Diligent::NativeWindow{window_handle};
}

bool glfw::should_close() const { return glfwWindowShouldClose(window); }

void glfw::poll_events() { glfwPollEvents(); }

std::unique_ptr<glfw> glfw::create() {
  if (!glfw_counter++)
    glfwInit();

  auto ret = std::unique_ptr<glfw>{new glfw};
  ret->window = glfwCreateWindow(640, 480, "awawawawawa", nullptr, nullptr);
  assert(ret->window && "failed to create window");
  return ret;
}

glfw::glfw() = default;
glfw::glfw(glfw &&rhs) noexcept : window{rhs.window} { rhs.window = nullptr; }
glfw &glfw::operator=(glfw &&rhs) noexcept {
  std::swap(window, rhs.window);
  return *this;
}
glfw::~glfw() noexcept {
  if (window) {
    glfwDestroyWindow(window);
    if (!--glfw_counter)
      glfwTerminate();
  }
};

} // namespace awawa