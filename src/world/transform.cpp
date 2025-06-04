#include "world/transform.h"

namespace awawa {
float4x4 transform::matrix() const {
  return float4x4::Scale(scale) * rotation.ToMatrix() *
         float4x4::Translation(position);
}
} // namespace awawa