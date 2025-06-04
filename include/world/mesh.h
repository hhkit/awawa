#pragma once
#include "core.h"
namespace awawa {
struct mesh {
  std::vector<float3> positions;
  std::vector<float4> colors;
  std::vector<uint32_t> indexes;
};
} // namespace awawa