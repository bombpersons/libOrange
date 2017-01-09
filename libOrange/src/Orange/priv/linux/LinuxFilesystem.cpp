#if defined __linux__

#include <Orange/util/Filesystem.hpp>

#include <dirent.h>
#include <stdio.h>
#include <string.h>

namespace orange {
  namespace filesystem {

    // Iterate over the files and directories in a folder.
    void IterateFiles(const char* _path, std::function<bool(const char*)> _func) {
      DIR* d;
      struct dirent* dir;
      d = opendir(_path);
      if (d) {
        while ((dir = readdir(d)) != nullptr) {
          if (dir->d_type == DT_REG) {
            std::string fullpath = std::string(_path) + "/" + dir->d_name;
            _func(fullpath.c_str());
          }
        }

        closedir(d);
      }

      return;
    }

    void IterateDirs(const char* _path, std::function<bool(const char*)> _func) {
      DIR* d;
      struct dirent* dir;
      d = opendir(_path);
      if (d) {
        while ((dir = readdir(d)) != nullptr) {
          if (dir->d_type == DT_DIR && strcmp(dir->d_name, "..") != 0 && strcmp(dir->d_name, ".") != 0) {
            std::string fullpath = std::string(_path) + "/" + dir->d_name;
            _func(fullpath.c_str());
          }
        }

        closedir(d);
      }

      return;
    }

  }
}

#endif
