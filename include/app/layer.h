#pragma once

#include "app/diligent.h"
#include "core.h"
namespace awawa {
class layer {
public:
  virtual void init(diligent &engine) = 0;
  virtual void update(seconds dt, diligent &engine) = 0;
  virtual void render(diligent &engine) = 0;
};
} // namespace awawa