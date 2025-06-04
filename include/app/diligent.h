#pragma once
#include "DeviceContext.h"
#include "EngineFactoryVk.h"
#include "RenderDevice.h"
#include "SwapChain.h"
#include "gfx/pipeline.h"

namespace awawa {
struct diligent {
  Diligent::IRenderDevice *device;
  Diligent::IDeviceContext *immediate_context;
  Diligent::ISwapChain *swapchain;

  void present();

  static diligent create(Diligent::NativeWindow window);
};
} // namespace awawa