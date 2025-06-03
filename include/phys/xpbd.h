#pragma once
#include "phys/softbody.h"
#include <span>

namespace awawa {
void solve(std::span<point_mass> points,
           std::span<const constraint> constraints);
}