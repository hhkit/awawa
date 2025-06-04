#include <cassert>

#include "app/application.h"
#include "app/diligent.h"
#include "app/glfw.h"
#include "app/os.h"
#include "gfx/pipeline.h"
#include "gfx/utils.h"

#include "tutorial/02_cube.h"

namespace awawa {

struct application::impl {
  bool is_running = false;
  std::unique_ptr<glfw::with_user_data<impl>> window;
  std::unique_ptr<diligent> diligent_engine;
  tutorial::_02::world_state world;

  impl(std::span<char *> args) {}
  ~impl() noexcept {
    if (is_running)
      cleanup();
  }

  void init() {
    assert(!is_running && "double initialization");

    init_glfw();
    init_diligent();

    world = tutorial::_02::create_world(*diligent_engine);
    world.init(*diligent_engine);

    is_running = true;
    diligent_engine->immediate_context->Flush();
    diligent_engine->immediate_context->FinishFrame();
  }
  void run() {
    using clock = std::chrono::steady_clock;
    auto prev = clock::now();

    while (is_running) {
      auto curr = clock::now();
      auto dt = std::chrono::duration_cast<seconds>(curr - prev);

      window->poll_events();
      world.update(dt);
      world.render(*diligent_engine);

      diligent_engine->present();
      is_running = !window->should_close();
      prev = curr;
    }
  }
  void cleanup() { is_running = false; }

private:
  void on_resize(int w, int h) {}

  void init_glfw() {
    window = glfw::create(this);
    window->add_callback<&impl::on_resize>(&glfwSetWindowSizeCallback);
  }

  void init_diligent() {
    diligent_engine = std::unique_ptr<diligent>{
        new diligent{diligent::create(window->native_window())}};
  }
};

void application::run() { self->run(); }

application::application(std::span<char *> args) : self{new impl{args}} {
  self->init();
}

application::~application() noexcept = default;
} // namespace awawa