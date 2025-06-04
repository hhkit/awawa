#pragma once

#include "world/mesh.h"
#include "world/transform.h"

namespace awawa {
struct mesh_renderer {
  mesh model;
  transform tfm;
};
} // namespace awawa