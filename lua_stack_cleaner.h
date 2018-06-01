#ifndef LUA_STACK_CLEANER_H_
#define LUA_STACK_CLEANER_H_

#include <mutex>

extern "C" {
#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"
}

class LuaStackCleaner {
public:
  LuaStackCleaner(lua_State* lua_state);
  ~LuaStackCleaner();

private:
  lua_State* lua_state_;
};

#endif // LUA_STACK_CLEANER_H_