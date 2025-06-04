#include "app/diligent.h"

namespace awawa {
diligent diligent::create(Diligent::NativeWindow window) {
  diligent ret;
  ret.window = window;

  // initialize vulkan
  Diligent::EngineVkCreateInfo engineCreateInfo{};

  auto factoryVk = Diligent::GetEngineFactoryVk();
  factoryVk->CreateDeviceAndContextsVk(engineCreateInfo, &ret.device,
                                       &ret.immediate_context);
  Diligent::SwapChainDesc swapChainDesc;
  swapChainDesc.BufferCount = 3;

  factoryVk->CreateSwapChainVk(ret.device, ret.immediate_context, swapChainDesc,
                               window, &ret.swapchain);

  return ret;
}

void diligent::present() { swapchain->Present(); }
} // namespace awawa