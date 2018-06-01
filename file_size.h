#ifndef LUA_FILE_SIZE_H_
#define LUA_FILE_SIZE_H_

#include <string>
#include "lua_function_interface.h"

class FileSize : public LuaFunctionInterface {
 public:
  std::string FunctionCall(const std::string& param);
};

#endif  // LUA_FILE_SIZE_H_