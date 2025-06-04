#pragma once

#include "RefCntAutoPtr.hpp"
#include "RenderDevice.h"
#include "Shader.h"
#include "SwapChain.h"
#include <filesystem>
#include <span>

namespace awawa {
using shader_ptr = Diligent::RefCntAutoPtr<Diligent::IShader>;
using pipeline_ptr = Diligent::RefCntAutoPtr<Diligent::IPipelineState>;
using shader_binding_ptr =
    Diligent::RefCntAutoPtr<Diligent::IShaderResourceBinding>;
using buffer_ptr = Diligent::RefCntAutoPtr<Diligent::IBuffer>;

using shader_stage = Diligent::SHADER_TYPE;

struct shader_factory {
  Diligent::RefCntAutoPtr<Diligent::IShaderSourceInputStreamFactory>
      shader_source_factory;
  Diligent::IRenderDevice *device;

  explicit shader_factory(Diligent::IRenderDevice *device);

  shader_ptr create_shader(std::filesystem::path shader_path,
                           shader_stage stage);
};

enum class buffer_type {
  vertex,
  index,
  uniform,
};

buffer_ptr create_buffer(Diligent::IRenderDevice *device,
                         std::span<const std::byte> buffer_data,
                         buffer_type buf_ty, Diligent::USAGE usage);

template <typename T>
buffer_ptr create_buffer(Diligent::IRenderDevice *device,
                         std::span<const T> buffer_data, buffer_type buf_ty,
                         Diligent::USAGE usage) {
  return create_buffer(
      device,
      std::span{reinterpret_cast<const std::byte *>(buffer_data.data()),
                buffer_data.size_bytes()},
      buf_ty, usage);
}

template <typename T>
buffer_ptr create_buffer(Diligent::IRenderDevice *device,
                         const std::vector<T> &buffer_data, buffer_type buf_ty,
                         Diligent::USAGE usage) {
  return create_buffer(device, std::span<const T>{buffer_data}, buf_ty, usage);
}
} // namespace awawa