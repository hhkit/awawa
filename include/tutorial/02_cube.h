#pragma once
#include "app/diligent.h"
#include "gfx/pipeline.h"
#include "world/mesh.h"
#include "world/transform.h"

namespace awawa::tutorial::_02 {
struct world_state {
  buffer_ptr vertices;
  buffer_ptr colors;
  buffer_ptr indices;

  pipeline_ptr pipeline;
  shader_binding_ptr srb;
  buffer_ptr uniforms;

  transform cube_transform;
  float4x4 wvMatrix, projMatrix;

  void init(diligent &engine);
  void update(seconds dt);
  void render(diligent &engine);
};

mesh create_cube();
world_state create_world(diligent &engine);

} // namespace awawa::tutorial::_02