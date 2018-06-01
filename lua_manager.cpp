#include "lua_manager.h"
#include "lua_function_interface.h"
#include "lua_stack_cleaner.h"

namespace {

LuaManager* g_this = NULL;

int g_CallCFunction(lua_State* lua_state) {
  if (g_this) return g_this->CallCFunction(lua_state);
  return 0;
}

int g_HaveCFunction(lua_State* lua_state) {
  if (g_this) return g_this->HaveCFunction(lua_state);
  return 0;
}

}  // namespace

LuaManager::LuaManager() : lua_state_(NULL) {
  g_this = this;
  lua_state_ = luaL_newstate();
  if (lua_state_) {
    luaL_openlibs(lua_state_);
    lua_pushcfunction(lua_state_, g_CallCFunction);
    lua_setglobal(lua_state_, "CallCFunction");
    lua_register(lua_state_, "CallCFunction", g_CallCFunction);
    lua_pushcfunction(lua_state_, g_HaveCFunction);
    lua_setglobal(lua_state_, "HaveCFunction");
    lua_register(lua_state_, "HaveCFunction", g_HaveCFunction);
  }
}

LuaManager::~LuaManager() {
  if (g_this == this) { 
    g_this = nullptr;
  }
  if (lua_state_) lua_close(lua_state_);
}

void LuaManager::SetCallCFunctionHook(const std::shared_ptr<CallCFunctionHookInterface> call_c_function_hook) {
  call_c_function_hook_ = call_c_function_hook;
}

void LuaManager::AddCFunction(const std::string& function_name, const std::shared_ptr<LuaFunctionInterface>& c_function) {
  std::unique_lock<std::mutex> lock(initialize_guard_);
  c_functions_[function_name] = c_function;
}

bool LuaManager::AddLuaCode(const std::string& lua_code) {
  std::unique_lock<std::mutex> lock(initialize_guard_);
  if (!lua_state_ || lua_code.empty()) return false;
  LuaStackCleaner stack_cleaner(lua_state_);
  if (luaL_loadbuffer(lua_state_, lua_code.c_str(), lua_code.length(), NULL) != 0) return false;
  if (lua_pcall(lua_state_, 0, LUA_MULTRET, 0) == 0) return true;
  return false;
}

bool LuaManager::CallLuaFunction(const std::string& function_name, const std::string& json_parameters, std::string* json_return) {
  bool ret_val = false;
  if (function_name.empty() || json_parameters.empty() || !json_return || !lua_state_) return ret_val;
  std::unique_lock<std::mutex> lock(initialize_guard_);
  LuaStackCleaner stack_cleaner(lua_state_);
  lua_getglobal(lua_state_, function_name.c_str());
  if (lua_isfunction(lua_state_, 1)) {
    lua_pushstring(lua_state_, json_parameters.c_str());
    if (lua_pcall(lua_state_, 1, 1, 0) == 0) {
      if (!lua_isnil(lua_state_, -1) && lua_isstring(lua_state_, -1)) {
        std::string tmp = lua_tostring(lua_state_, -1);
        json_return->assign(tmp.begin(), tmp.end());
        ret_val = true;
      }
    } else {
      printf("FATAL ERROR:\n  %s\n\n", lua_tostring(lua_state_, -1));
    }
  }
  return ret_val;
}

bool LuaManager::CallLuaFunction(const std::string& function_name, std::string* json_return) {
  bool ret_val = false;
  if (function_name.empty() || !json_return || !lua_state_) return ret_val;
  std::unique_lock<std::mutex> lock(initialize_guard_);
  LuaStackCleaner stack_cleaner(lua_state_);
  lua_getglobal(lua_state_, function_name.c_str());
  if (lua_isfunction(lua_state_, 1)) {
    if (lua_pcall(lua_state_, 0, 1, 0) == 0) {
      if (!lua_isnil(lua_state_, -1) && lua_isstring(lua_state_, -1)) {
        std::string tmp = lua_tostring(lua_state_, -1);
        json_return->assign(tmp.begin(), tmp.end());
        ret_val = true;
      }
    }
  }
  return ret_val;
}

int LuaManager::CallCFunction(lua_State* lua_state) {
  if (!lua_state) return 0;
  int number_of_parameters = lua_gettop(lua_state);
  if (number_of_parameters == 2) {
    std::string func_name = lua_tostring(lua_state, 1);
    std::string param = lua_tostring(lua_state, 2);
    std::map<std::string, std::shared_ptr<LuaFunctionInterface> >::iterator it =
        c_functions_.find(func_name);
    if (it != c_functions_.end()) {
      std::string ret = it->second->FunctionCall(param);
      lua_pushstring(lua_state, ret.c_str());

      if (call_c_function_hook_ != NULL) {
        call_c_function_hook_->Call(func_name, param, ret);
      }
      return 1;
    }
  }
  return 0;
}

int LuaManager::HaveCFunction(lua_State* lua_state) {
  if (!lua_state) return 0;
  int number_of_parameters = lua_gettop(lua_state);
  if (number_of_parameters == 1) {
    std::string func_name = lua_tostring(lua_state, 1);
    std::map<std::string, std::shared_ptr<LuaFunctionInterface> >::iterator it =
        c_functions_.find(func_name);
    lua_pushboolean(lua_state, it != c_functions_.end());
    return 1;
  }
  return 0;
}
