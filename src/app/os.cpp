#include "app/os.h"

#include <vector>

#if defined(_WIN32)
#include <windows.h>
#elif defined(__linux__)
#include <sstream>
#include <unistd.h>
#elif defined(__APPLE__)
#include <mach-o/dyld.h>
#endif

namespace awawa {

std::filesystem::path find_executable() {
#if defined(_WIN32)
  unsigned int bufferSize = MAX_PATH;
#elif defined(__linux__)
  unsigned int bufferSize = MAX_PATH;
#elif defined(__APPLE__)
  unsigned int bufferSize = PATH_MAX;
#endif
  std::vector<char> buffer(bufferSize + 1);

#if defined(_WIN32)
  ::GetModuleFileName(NULL, &buffer[0], bufferSize);

#elif defined(__linux__)
  // Get the process ID.
  int pid = getpid();

  // Construct a path to the symbolic link pointing to the process executable.
  // This is at /proc/<pid>/exe on Linux systems (we hope).
  std::ostringstream oss;
  oss << "/proc/" << pid << "/exe";
  std::string link = oss.str();

  // Read the contents of the link.
  int count = readlink(link.c_str(), &buffer[0], bufferSize);
  if (count == -1)
    throw std::runtime_error("Could not read symbolic link");
  buffer[count] = '\0';

#elif defined(__APPLE__)
  if (_NSGetExecutablePath(&buffer[0], &bufferSize)) {
    buffer.resize(bufferSize);
    _NSGetExecutablePath(&buffer[0], &bufferSize);
  }

#else
#error Cannot yet find the executable on this platform
#endif

  std::string s = &buffer[0];
  return s;
}

std::filesystem::path find_executable_directory() {
  return find_executable().parent_path();
}

} // namespace awawa