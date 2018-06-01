#ifndef LUA_MANAGER_H_
#define LUA_MANAGER_H_

#include <string>
#include <map>
#include <memory>
#include <mutex>

#include "lua_function_interface.h"
#include "call_c_function_hook.h"

#pragma comment(lib, "lua5.1.lib")

extern "C" {
#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"
}

class LuaManager {
 public:
  LuaManager();
  virtual ~LuaManager();

  void SetCallCFunctionHook(const std::shared_ptr<CallCFunctionHookInterface> call_c_function_hook);

  void AddCFunction(const std::string& function_name, const std::shared_ptr<LuaFunctionInterface>& c_function);
  bool AddLuaCode(const std::string& lua_code);
  bool CallLuaFunction(const std::string& function_name, const std::string& string_json,
                       std::string* string_json_return);
  bool CallLuaFunction(const std::string& function_name, std::string* string_json_return);

  int CallCFunction(lua_State* lua_state);
  int HaveCFunction(lua_State* lua_state);

 private:
  std::mutex initialize_guard_;
  lua_State* lua_state_;
  std::map<std::string, std::shared_ptr<LuaFunctionInterface> > c_functions_;
  std::shared_ptr<CallCFunctionHookInterface> call_c_function_hook_;
};

#endif  // LUA_MANAGER_H_
