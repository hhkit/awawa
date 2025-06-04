#include "tutorial/02_cube.h"
#include "MapHelper.hpp"
#include <array>

namespace awawa::tutorial::_02 {

namespace {
static float4x4 get_pretransform(const Diligent::SwapChainDesc &desc,
                                 const float3 &axis) {
  switch (desc.PreTransform) {
  case Diligent::SURFACE_TRANSFORM_ROTATE_90:
    // The image content is rotated 90 degrees clockwise.
    return float4x4::RotationArbitrary(axis, -pi / 2.f);

  case Diligent::SURFACE_TRANSFORM_ROTATE_180:
    // The image content is rotated 180 degrees clockwise.
    return float4x4::RotationArbitrary(axis, -pi);

  case Diligent::SURFACE_TRANSFORM_ROTATE_270:
    // The image content is rotated 270 degrees clockwise.
    return float4x4::RotationArbitrary(axis, -pi * 3.f / 2.f);

  case Diligent::SURFACE_TRANSFORM_OPTIMAL:
    UNEXPECTED("SURFACE_TRANSFORM_OPTIMAL is only valid as parameter during "
               "swap chain initialization.");
    return float4x4::Identity();

  case Diligent::SURFACE_TRANSFORM_HORIZONTAL_MIRROR:
  case Diligent::SURFACE_TRANSFORM_HORIZONTAL_MIRROR_ROTATE_90:
  case Diligent::SURFACE_TRANSFORM_HORIZONTAL_MIRROR_ROTATE_180:
  case Diligent::SURFACE_TRANSFORM_HORIZONTAL_MIRROR_ROTATE_270:
    UNEXPECTED("Mirror transforms are not supported");
    return float4x4::Identity();

  default:
    return float4x4::Identity();
  }
}

static float4x4 project(const diligent &engine, float fov, float near,
                        float far) {
  const auto &swap_desc = engine.swapchain->GetDesc();
  const auto device = engine.device;

  float AspectRatio = static_cast<float>(swap_desc.Width) /
                      static_cast<float>(swap_desc.Height);
  float XScale, YScale;
  if (swap_desc.PreTransform == Diligent::SURFACE_TRANSFORM_ROTATE_90 ||
      swap_desc.PreTransform == Diligent::SURFACE_TRANSFORM_ROTATE_270 ||
      swap_desc.PreTransform ==
          Diligent::SURFACE_TRANSFORM_HORIZONTAL_MIRROR_ROTATE_90 ||
      swap_desc.PreTransform ==
          Diligent::SURFACE_TRANSFORM_HORIZONTAL_MIRROR_ROTATE_270) {
    // When the screen is rotated, vertical FOV becomes horizontal FOV
    XScale = 1.f / std::tan(fov / 2.f);
    // Aspect ratio is inversed
    YScale = XScale * AspectRatio;
  } else {
    YScale = 1.f / std::tan(fov / 2.f);
    XScale = YScale / AspectRatio;
  }

  float4x4 Proj;
  Proj._11 = XScale;
  Proj._22 = YScale;
  Proj.SetNearFarClipPlanes(near, far, device->GetDeviceInfo().NDC.MinZ == -1);
  return Proj;
}
} // namespace

mesh create_cube() {
  mesh m{{
             float3(-1, -1, -1), //
             float3(-1, +1, -1), //
             float3(+1, +1, -1), //
             float3(+1, -1, -1), //

             float3(-1, -1, +1), //
             float3(-1, +1, +1), //
             float3(+1, +1, +1), //
             float3(+1, -1, +1), //
         },
         {
             float4{1, 0, 0, 1},
             float4{0, 1, 0, 1},
             float4{0, 0, 1, 1},
             float4{1, 1, 1, 1},
             float4{1, 1, 0, 1},
             float4{0, 1, 1, 1},
             float4{1, 0, 1, 1},
             float4{0.2f, 0.2f, 0.2f, 1.f},
         },
         {
             2, 0, 1, 2, 3, 0, //
             4, 6, 5, 4, 7, 6, //
             0, 7, 4, 0, 3, 7, //
             1, 0, 4, 1, 4, 5, //
             1, 5, 2, 5, 6, 2, //
             3, 6, 7, 3, 2, 6, //
         }};
  return m;
}

world_state create_world(diligent &engine) {
  world_state world;
  auto cube = create_cube();

  world.vertices =
      create_buffer(engine.device, cube.positions, buffer_type::vertex,
                    Diligent::USAGE_IMMUTABLE);

  world.colors = create_buffer(engine.device, cube.colors, buffer_type::vertex,
                               Diligent::USAGE_IMMUTABLE);

  world.indices = create_buffer(engine.device, cube.indexes, buffer_type::index,
                                Diligent::USAGE_IMMUTABLE);

  world.cube_transform.rotation =
      quat::RotationFromAxisAngle({1, 0, 0}, -pi * 0.1f);

  return world;
}

void world_state::init(diligent &engine) {
  std::cout << "initializing render resources" << std::endl;

  std::cout << "    shaders" << std::endl;

  shader_factory factory{engine.device};
  auto vs =
      factory.create_shader("02_cube.vert", shader_stage::SHADER_TYPE_VERTEX);

  auto fs =
      factory.create_shader("02_cube.frag", shader_stage::SHADER_TYPE_PIXEL);

  auto mat = cube_transform.matrix();
  uniforms = create_buffer(engine.device, std::span<const float4x4>{&mat, 2},
                           buffer_type::uniform, Diligent::USAGE_DYNAMIC);

  Diligent::GraphicsPipelineStateCreateInfo pso_ci;
  pso_ci.PSODesc.Name = "Cube PSO";
  pso_ci.PSODesc.PipelineType = Diligent::PIPELINE_TYPE_GRAPHICS;
  pso_ci.GraphicsPipeline.NumRenderTargets = 1;
  pso_ci.GraphicsPipeline.RTVFormats[0] =
      engine.swapchain->GetDesc().ColorBufferFormat;
  pso_ci.GraphicsPipeline.DSVFormat =
      engine.swapchain->GetDesc().DepthBufferFormat;
  pso_ci.GraphicsPipeline.PrimitiveTopology =
      Diligent::PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
  pso_ci.GraphicsPipeline.RasterizerDesc.CullMode = Diligent::CULL_MODE_BACK;
  pso_ci.GraphicsPipeline.DepthStencilDesc.DepthEnable = true;

  Diligent::LayoutElement LayoutElems[] = {
      // Attribute 0 - vertex position
      Diligent::LayoutElement{0, 0, 3, Diligent::VT_FLOAT32, false},
      Diligent::LayoutElement{1, 1, 4, Diligent::VT_FLOAT32, false},

  };

  pso_ci.GraphicsPipeline.InputLayout.LayoutElements = LayoutElems;
  pso_ci.GraphicsPipeline.InputLayout.NumElements = _countof(LayoutElems);

  pso_ci.pVS = vs;
  pso_ci.pPS = fs;
  pso_ci.PSODesc.ResourceLayout.DefaultVariableType =
      Diligent::SHADER_RESOURCE_VARIABLE_TYPE_STATIC;

  std::cout << "    pipeline" << std::endl;
  engine.device->CreateGraphicsPipelineState(pso_ci, &pipeline);
  pipeline->GetStaticVariableByIndex(Diligent::SHADER_TYPE_VERTEX, 0)
      ->Set(uniforms);
  pipeline->CreateShaderResourceBinding(&srb, true);

  std::cout << "    done" << std::endl;
}

void world_state::update(seconds dt, diligent &engine) {
  cube_transform.rotation *= quat::RotationFromAxisAngle({0, 1, 0}, dt.count());
}

void world_state::render(diligent &engine) {
  {
    auto &swap_desc = engine.swapchain->GetDesc();
    auto view = float4x4::Translation(0, 0, 5);
    auto ar = static_cast<float>(swap_desc.Width) /
              static_cast<float>(swap_desc.Height);
    auto pretransform = get_pretransform(swap_desc, {0, 0, 1});
    auto proj = project(engine, pi / 4.0f, 0.1, 100);

    wvMatrix = cube_transform.matrix() * view * pretransform;
    projMatrix = proj;
  }

  auto pRTV = engine.swapchain->GetCurrentBackBufferRTV();
  auto pDSV = engine.swapchain->GetDepthBufferDSV();
  // Clear the back buffer
  float4 ClearColor = {0.350f, 0.350f, 1.f, 1.0f};
  engine.immediate_context->SetRenderTargets(
      1, &pRTV, pDSV, Diligent::RESOURCE_STATE_TRANSITION_MODE_TRANSITION);

  engine.immediate_context->ClearRenderTarget(
      pRTV, ClearColor.Data(),
      Diligent::RESOURCE_STATE_TRANSITION_MODE_TRANSITION);
  engine.immediate_context->ClearDepthStencil(
      pDSV, Diligent::CLEAR_DEPTH_FLAG, 1.f, 0,
      Diligent::RESOURCE_STATE_TRANSITION_MODE_TRANSITION);

  {
    // Map the buffer and write current world-view-projection matrix
    Diligent::MapHelper<std::array<float4x4, 2>> CBConstants(
        engine.immediate_context, uniforms, Diligent::MAP_WRITE,
        Diligent::MAP_FLAG_DISCARD);
    std::array<float4x4, 2> matrices{wvMatrix, projMatrix};
    *CBConstants = matrices;
  }

  // Bind vertex and index buffers
  Diligent::IBuffer *pBuffs[] = {vertices, colors};
  engine.immediate_context->SetVertexBuffers(
      0, 2, pBuffs, nullptr,
      Diligent::RESOURCE_STATE_TRANSITION_MODE_TRANSITION,
      Diligent::SET_VERTEX_BUFFERS_FLAG_RESET);
  engine.immediate_context->SetIndexBuffer(
      indices, 0, Diligent::RESOURCE_STATE_TRANSITION_MODE_TRANSITION);

  // Set the pipeline state
  engine.immediate_context->SetPipelineState(pipeline);
  // Commit shader resources. RESOURCE_STATE_TRANSITION_MODE_TRANSITION mode
  // makes sure that resources are transitioned to required states.
  engine.immediate_context->CommitShaderResources(
      srb, Diligent::RESOURCE_STATE_TRANSITION_MODE_TRANSITION);

  Diligent::DrawIndexedAttribs DrawAttrs;    // This is an indexed draw call
  DrawAttrs.IndexType = Diligent::VT_UINT32; // Index type
  DrawAttrs.NumIndices = 36;
  // Verify the state of vertex and index buffers
  DrawAttrs.Flags = Diligent::DRAW_FLAG_VERIFY_ALL;
  engine.immediate_context->DrawIndexed(DrawAttrs);
}
} // namespace awawa::tutorial::_02