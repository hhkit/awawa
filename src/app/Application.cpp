#include <cassert>

#include "app/application.h"
#include "app/diligent.h"
#include "app/glfw.h"

namespace awawa {

struct application::impl {
  bool is_running = false;
  std::unique_ptr<glfw> window;
  std::unique_ptr<diligent> diligent_engine;

  impl(std::span<char *> args) {}
  ~impl() noexcept {
    if (is_running)
      cleanup();
  }

  void init() {
    assert(!is_running && "double initialization");

    init_glfw();
    init_vulkan();

    is_running = true;
  }
  void run() {
    while (is_running) {
      window->poll_events();
      is_running = !window->should_close();
    }
  }
  void cleanup() { is_running = false; }

private:
  void init_glfw() { window = glfw::create(); }
  void init_vulkan() {
    diligent_engine = std::unique_ptr<diligent>{
        new diligent{diligent::create(window->native_window())}};
  }
};

void application::run() { self->run(); }

application::application(std::span<char *> args) : self{new impl{args}} {
  self->init();
  self->run();
  self->cleanup();
}

application::~application() noexcept = default;
} // namespace awawa