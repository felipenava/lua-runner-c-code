#include "lua_stack_cleaner.h"

LuaStackCleaner::LuaStackCleaner(lua_State* lua_state) : lua_state_(lua_state) {
}

LuaStackCleaner::~LuaStackCleaner() {
  if (lua_state_) {
    int size = lua_gettop(lua_state_);
    if (size) lua_pop(lua_state_, size);
  }
}