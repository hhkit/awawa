#include <cassert>

#include "app/application.h"
#include "app/diligent.h"
#include "app/glfw.h"
#include "app/os.h"
#include "gfx/pipeline.h"
#include "gfx/utils.h"

namespace awawa {

struct application::impl {
  bool is_running = false;
  std::unique_ptr<glfw::with_user_data<impl>> window;
  std::unique_ptr<diligent> diligent_engine;
  pipeline_ptr pipeline;

  impl(std::span<char *> args) {}
  ~impl() noexcept {
    if (is_running)
      cleanup();
  }

  void init() {
    assert(!is_running && "double initialization");

    init_glfw();
    init_diligent();
    init_triangle_pipeline();

    is_running = true;
  }
  void run() {
    while (is_running) {
      window->poll_events();
      diligent_engine->render(pipeline);
      diligent_engine->present();
      is_running = !window->should_close();
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
  void init_triangle_pipeline() {
    auto shader_dir = find_executable_directory() / "shaders";
    auto vs_code = loadTextFile(shader_dir / "tutorial.vert");
    auto fs_code = loadTextFile(shader_dir / "tutorial.frag");

    if (!(vs_code && fs_code))
      throw std::runtime_error("shaders not found");

    auto vs = createShader(diligent_engine->device, *vs_code,
                           shader_stage::SHADER_TYPE_VERTEX);

    auto fs = createShader(diligent_engine->device, *fs_code,
                           shader_stage::SHADER_TYPE_PIXEL);

    if (!(vs && fs))
      throw std::runtime_error("shaders not loaded");

    pipeline = createPipeline(diligent_engine->device,
                              diligent_engine->swapchain, vs, fs);
  }
};

void application::run() { self->run(); }

application::application(std::span<char *> args) : self{new impl{args}} {
  self->init();
}

application::~application() noexcept = default;
} // namespace awawa