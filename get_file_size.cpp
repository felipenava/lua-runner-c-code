#include <windows.h>
#include <string>
#include "get_file_size.h"

namespace utilities {

unsigned int GetFileSize(const std::wstring& file_path) {
  if (file_path.empty()) return 0;
  HANDLE file_handle = INVALID_HANDLE_VALUE;
  unsigned int ret_val = 0;
  file_handle = CreateFileW(file_path.c_str(), GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING,
                            0, NULL);
  if (file_handle == INVALID_HANDLE_VALUE) return 0;
  ret_val = ::GetFileSize(file_handle, NULL);
  if (ret_val == INVALID_FILE_SIZE) ret_val = 0;
  CloseHandle(file_handle);
  return ret_val;
}

}  // namespace utilities
