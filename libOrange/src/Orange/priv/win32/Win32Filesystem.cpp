#include <Orange/util/Filesystem.hpp>

#include <Windows.h>

namespace orange {
  namespace filesystem {



    // Iterate over the files and directories in a folder.
    void IterateFiles(const char* _path, std::function<bool(const char*)> _func) {
      std::string findPath = std::string(_path) + "/*";

      WIN32_FIND_DATA findFileData;
      HANDLE handle = FindFirstFile(findPath.c_str(), &findFileData);
      if (handle != INVALID_HANDLE_VALUE) {
        do {
          if (findFileData.dwFileAttributes != FILE_ATTRIBUTE_DIRECTORY) {
            std::string path = std::string(_path) + "/" + findFileData.cFileName;
            if (!_func(path.c_str()))
              break;
          }
        } while (FindNextFile(handle, &findFileData));

        FindClose(handle);
        if (GetLastError() != ERROR_NO_MORE_FILES) {
          // Error...
        }
      }
      return;
    }

    void IterateDirs(const char* _path, std::function<bool(const char*)> _func) {
      std::string findPath = std::string(_path) + "/*";

      WIN32_FIND_DATA findFileData;
      HANDLE handle = FindFirstFile(findPath.c_str(), &findFileData);
      if (handle != INVALID_HANDLE_VALUE) {
        do {
          if (findFileData.dwFileAttributes == FILE_ATTRIBUTE_DIRECTORY) {
            std::string path = std::string(_path) + "/" + findFileData.cFileName;
            if (!_func(path.c_str()))
              break;
          }
        } while (FindNextFile(handle, &findFileData));

        FindClose(handle);
        if (GetLastError() != ERROR_NO_MORE_FILES) {
          // Error...
        }
      }
      return;
    }

  }
}
