#include "app/diligent.h"

namespace awawa {
diligent diligent::create(Diligent::NativeWindow window) {
  diligent ret;
  // initialize vulkan
  Diligent::EngineVkCreateInfo engineCreateInfo{};

  auto factoryVk = Diligent::GetEngineFactoryVk();
  factoryVk->CreateDeviceAndContextsVk(engineCreateInfo, &ret.device,
                                       &ret.immediate_context);
  Diligent::SwapChainDesc swapChainDesc;

  factoryVk->CreateSwapChainVk(ret.device, ret.immediate_context, swapChainDesc,
                               window, &ret.swapchain);

  return ret;
}

// void diligent::render(pipeline_ptr pipeline) {
//   auto *pRTV = swapchain->GetCurrentBackBufferRTV();
//   auto *pDSV = swapchain->GetDepthBufferDSV();
//   immediate_context->SetRenderTargets(
//       1, &pRTV, pDSV, Diligent::RESOURCE_STATE_TRANSITION_MODE_TRANSITION);

//   const float ClearColor[] = {0.350f, 0.350f, 0.350f, 1.0f};

//   immediate_context->ClearRenderTarget(
//       pRTV, ClearColor, Diligent::RESOURCE_STATE_TRANSITION_MODE_TRANSITION);
//   immediate_context->ClearDepthStencil(
//       pDSV, Diligent::CLEAR_DEPTH_FLAG, 1.f, 0,
//       Diligent::RESOURCE_STATE_TRANSITION_MODE_TRANSITION);

//   immediate_context->SetPipelineState(pipeline);

//   // Typically we should now call CommitShaderResources(), however shaders in
//   // this example don't use any resources.

//   Diligent::DrawAttribs drawAttrs;
//   drawAttrs.NumVertices = 3; // We will render 3 vertices
//   immediate_context->Draw(drawAttrs);
//   // immediate_context->Flush();
// }

void diligent::present() { swapchain->Present(); }
} // namespace awawa