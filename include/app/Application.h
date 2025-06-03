#pragma once
#include <memory>
#include <span>

namespace awawa {
class application {
public:
  application(std::span<char *> args);
  ~application() noexcept;

  void run();

  application(application &&) = delete;
  application(const application &) = delete;
  application &operator=(application &&) = delete;
  application &operator=(const application &) = delete;

private:
  struct impl;
  std::unique_ptr<impl> self;
};
}; // namespace awawa