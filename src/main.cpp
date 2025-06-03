#include <memory>

#include "app/application.h"

int main(int argc, char **argv) {
  using namespace awawa;
  std::unique_ptr<application> app(
      new application{std::span{argv, static_cast<size_t>(argc)}});
  app->run();
}