#include "gui/gui.h"
#include "ImGuiImplMacOS.hpp"

namespace awawa {
void imgui::init(diligent &engine) {
  Diligent::ImGuiDiligentCreateInfo ci{engine.device,
                                       engine.swapchain->GetDesc()};
  impl = Diligent::ImGuiImplMacOS::Create(ci, engine.window.pNSView);
}

void imgui::update(seconds dt, diligent &engine) {
  const auto &swapchain_desc = engine.swapchain->GetDesc();
  impl->NewFrame(swapchain_desc.Width, swapchain_desc.Height,
                 swapchain_desc.PreTransform);

  // other code here
  // ImGui::BeginGroup();
  ImGui::ShowDebugLogWindow();
}

void imgui::render(diligent &engine) {
  const auto ctx = engine.immediate_context;

  ctx->ClearStats();
  impl->Render(ctx);
}
} // namespace awawa