#ifndef ORANGE_FILESYSTEM
#define ORANGE_FILESYSTEM

#include <functional>

namespace orange {
  namespace filesystem {
    // Iterate over the files and directories in a folder.
    void IterateFiles(const char* _path, std::function<bool(const char*)> _func);
    void IterateDirs(const char* _path, std::function<bool(const char*)> _func);
  }
}

#endif
