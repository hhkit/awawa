#pragma once
#include <type_traits>

#include "app/diligent.h"
#include "app/layer.h"
#include "core.h"
#include "imgui.h"

#include "ImGuiDiligentRenderer.hpp"
#include "ImGuiImplDiligent.hpp"

namespace awawa {
struct imgui : layer {
  std::unique_ptr<Diligent::ImGuiImplDiligent> impl;

  void init(diligent &engine) override;
  void update(seconds dt, diligent &engine) override;
  void render(diligent &engine) override;
};
} // namespace awawa