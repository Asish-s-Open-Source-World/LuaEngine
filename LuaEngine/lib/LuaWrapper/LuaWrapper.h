#ifndef LUA_WRAPPER_H
#define LUA_WRAPPER_H

#include <Arduino.h>

// #define LUA_USE_C89
#include "lua.hpp"

/**
 * @brief Wrap Lua library for executing scripts
 * 
 */
class LuaWrapper {
  private:
  
  lua_State *_state;

  public:

  void LW_ResetLVM();
  void LW_RegisterFunc(const char *name, const lua_CFunction function);
  void LW_ExecuteFile(const char *filename, bool close_LVM = 0);
  void LW_GarbCollectFull();
};

#endif