#include "file_size.h"
#include "get_file_size.h"

std::string FileSize::FunctionCall(const std::string& param) {
  std::wstring file_path_unicode(param.begin(), param.end());
  int ret_value = utilities::GetFileSize(file_path_unicode);
  return std::to_string(ret_value);
}
