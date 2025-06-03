#pragma once

#include "RefCntAutoPtr.hpp"
#include "RenderDevice.h"
#include "Shader.h"
#include "SwapChain.h"
#include <optional>

namespace awawa {
using shader_ptr = Diligent::RefCntAutoPtr<Diligent::IShader>;
using pipeline_ptr = Diligent::RefCntAutoPtr<Diligent::IPipelineState>;

using shader_stage = Diligent::SHADER_TYPE;

shader_ptr createShader(Diligent::IRenderDevice *device,
                        std::string_view shader_code, shader_stage stage);

pipeline_ptr createPipeline(Diligent::IRenderDevice *device,
                            Diligent::ISwapChain *swapchain,
                            shader_ptr vertex_shader,
                            shader_ptr fragment_shader);
} // namespace awawa