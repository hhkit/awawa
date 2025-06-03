#pragma once
#include <vector>
namespace awawa {
struct point_mass {};
struct constraint {};

struct softbody {
  std::vector<point_mass> points;
  std::vector<constraint> constraints;
};
} // namespace awawa