#pragma once
#include "core.h"
namespace awawa {
struct transform {
  float3 position;
  quat rotation{0, 0, 0, 1};
  float3 scale{1, 1, 1};

  float4x4 matrix() const;
};
} // namespace awawa