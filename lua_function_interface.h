#ifndef LUA_FUNCTION_INTERFACE_H_
#define LUA_FUNCTION_INTERFACE_H_

#include <string>

class LuaFunctionInterface {
 public:
  virtual ~LuaFunctionInterface() {};
  virtual std::string FunctionCall(const std::string& param) = 0;
};

#endif  // LUA_FUNCTION_INTERFACE_H_
