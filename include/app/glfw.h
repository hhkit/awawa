#pragma once
#include <functional>
#include <memory>

#include "GLFW/glfw3.h"
#include "NativeWindow.h"

namespace awawa {
struct glfw {
  template <typename T> struct with_user_data;

  GLFWwindow *window{};

  Diligent::NativeWindow native_window() const;
  bool should_close() const;
  void poll_events();

  static std::unique_ptr<glfw> create();
  template <typename T>
  static std::unique_ptr<with_user_data<T>> create(T *user_data);

  glfw();
  glfw(glfw &&) noexcept;
  glfw &operator=(glfw &&) noexcept;
  ~glfw() noexcept;
};

template <typename T> struct glfw::with_user_data : glfw {
  using glfw::glfw;
  using glfw::operator=;
  with_user_data(glfw &&ctx) : glfw{std::move(ctx)} {}

  T *get_data() { return static_cast<T *>(glfwGetWindowUserPointer(window)); }

  template <auto MemFn, typename Ret, typename... Args>
  auto add_callback(auto (*glfwSetFunc)(GLFWwindow *window,
                                        Ret (*)(GLFWwindow *, Args...))) {
    return glfwSetFunc(
        window, +[](GLFWwindow *window, Args... args) {
          auto ptr = static_cast<T *>(glfwGetWindowUserPointer(window));
          std::invoke(MemFn, ptr, args...);
        });
  };
};

template <typename T>
std::unique_ptr<glfw::with_user_data<T>> glfw::create(T *user_data) {
  auto ptr = create();
  glfwSetWindowUserPointer(ptr->window, user_data);
  return std::unique_ptr<glfw::with_user_data<T>>{
      new with_user_data<T>{std::move(*ptr)}};
}
} // namespace awawa