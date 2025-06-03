#include "gfx/pipeline.h"
#include "SwapChain.h"

namespace awawa {
shader_ptr createShader(Diligent::IRenderDevice *device,
                        std::string_view shader_code, shader_stage stage) {
  Diligent::ShaderCreateInfo ci;
  ci.SourceLanguage = Diligent::SHADER_SOURCE_LANGUAGE_HLSL;
  ci.Desc.UseCombinedTextureSamplers = true;
  ci.Source = shader_code.data();

  ci.Desc.ShaderType = stage;
  switch (stage) {
  case shader_stage::SHADER_TYPE_COMPUTE:
    ci.EntryPoint = "computeMain";
    ci.Desc.Name = "Compute shader";
    break;
  case shader_stage::SHADER_TYPE_VERTEX:
    ci.EntryPoint = "vertexMain";
    ci.Desc.Name = "Vertex shader";
    break;
  case shader_stage::SHADER_TYPE_PIXEL:
    ci.EntryPoint = "fragmentMain";
    ci.Desc.Name = "Pixel shader";
    break;
  default:
    throw std::runtime_error("unsupported shader stage");
  }

  shader_ptr ret;
  device->CreateShader(ci, &ret);
  return ret;
}

pipeline_ptr createPipeline(Diligent::IRenderDevice *device,
                            Diligent::ISwapChain *swapchain,
                            shader_ptr vertex_shader,
                            shader_ptr fragment_shader) {
  Diligent::GraphicsPipelineStateCreateInfo ci;
  Diligent::PipelineStateDesc &PSODesc = ci.PSODesc;

  PSODesc.Name = "Simple triangle PSO";
  PSODesc.PipelineType = Diligent::PIPELINE_TYPE_GRAPHICS;
  ci.GraphicsPipeline.NumRenderTargets = 1;
  ci.GraphicsPipeline.RTVFormats[0] = swapchain->GetDesc().ColorBufferFormat;

  ci.GraphicsPipeline.DSVFormat = Diligent::TEX_FORMAT_D32_FLOAT;
  ci.GraphicsPipeline.PrimitiveTopology =
      Diligent::PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;

  ci.GraphicsPipeline.RasterizerDesc.CullMode = Diligent::CULL_MODE_NONE;
  ci.GraphicsPipeline.DepthStencilDesc.DepthEnable = false;

  ci.pVS = vertex_shader;
  ci.pPS = fragment_shader;

  pipeline_ptr retval;
  device->CreatePipelineState(ci, &retval);
  return retval;
}
} // namespace awawa