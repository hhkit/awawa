#include "gfx/pipeline.h"
#include "app/os.h"

namespace awawa {
shader_factory::shader_factory(Diligent::IRenderDevice *device)
    : device{device} {
  device->GetEngineFactory()->CreateDefaultShaderSourceStreamFactory(
      (find_executable_directory() / "shaders").c_str(),
      &shader_source_factory);
}

shader_ptr shader_factory::create_shader(std::filesystem::path shader_path,
                                         shader_stage stage) {
  Diligent::ShaderCreateInfo ci;
  ci.pShaderSourceStreamFactory = shader_source_factory;
  ci.SourceLanguage = Diligent::SHADER_SOURCE_LANGUAGE_HLSL;
  ci.Desc.UseCombinedTextureSamplers = true;

  ci.FilePath = shader_path.c_str();

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
    throw std::runtime_error("unsupported shader stage in " +
                             shader_path.string());
  }

  shader_ptr ret;
  device->CreateShader(ci, &ret);
  if (!ret)
    throw std::runtime_error("shader compilation error in " +
                             shader_path.string());
  return ret;
}

buffer_ptr create_buffer(Diligent::IRenderDevice *device,
                         std::span<const std::byte> buffer_data,
                         buffer_type buf_ty, Diligent::USAGE usage) {
  buffer_ptr ptr;
  Diligent::BufferDesc buff_desc;
  buff_desc.Size = buffer_data.size();
  buff_desc.Usage = usage; // todo: compute

  Diligent::BufferData buffer_data_ci;
  buffer_data_ci.pData = buffer_data.data();
  buffer_data_ci.DataSize = buffer_data.size();

  switch (buf_ty) {
  case buffer_type::vertex:
    buff_desc.Name = "vertex buffer";
    buff_desc.BindFlags = Diligent::BIND_VERTEX_BUFFER;
    break;
  case buffer_type::index:
    buff_desc.Name = "index buffer";
    buff_desc.BindFlags = Diligent::BIND_INDEX_BUFFER;
    break;
  case buffer_type::uniform:
    buff_desc.Name = "uniform buffer";
    buff_desc.BindFlags = Diligent::BIND_UNIFORM_BUFFER;
    buff_desc.CPUAccessFlags = Diligent::CPU_ACCESS_WRITE;

    buffer_data_ci.pData = nullptr;
    break;
  };

  device->CreateBuffer(buff_desc, &buffer_data_ci, &ptr);
  return ptr;
}
} // namespace awawa