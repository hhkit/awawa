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
} // namespace awawa