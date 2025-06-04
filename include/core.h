#pragma once
#include <type_traits>

#include "BasicMath.hpp"
namespace awawa {
using Diligent::float2;
using Diligent::float3;
using Diligent::float4;
using Diligent::float4x4;

using quat = Diligent::QuaternionF;

using seconds = std::chrono::duration<float>;

static constexpr auto pi = Diligent::PI;
} // namespace awawa