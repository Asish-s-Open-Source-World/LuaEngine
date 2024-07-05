#include "LuaWrapper.h"

/**
 * @brief Start a new Lua virtual machine
 * 
 */
void LuaWrapper::LW_ResetLVM() {
  _state = luaL_newstate();

  // Uncomment required libraries
  static const luaL_Reg loadedlibs[] = {
    {LUA_GNAME, luaopen_base},
    // {LUA_LOADLIBNAME, luaopen_package},
    // {LUA_COLIBNAME, luaopen_coroutine},
    {LUA_TABLIBNAME, luaopen_table},
    // {LUA_IOLIBNAME, luaopen_io},
    // {LUA_OSLIBNAME, luaopen_os},
    {LUA_STRLIBNAME, luaopen_string},
    {LUA_MATHLIBNAME, luaopen_math},
    // {LUA_UTF8LIBNAME, luaopen_utf8},
    // {LUA_DBLIBNAME, luaopen_debug},
    {NULL, NULL}
  };

  // Register required libraries
  const luaL_Reg *lib;
  /* "require" functions from 'loadedlibs' and set results to global table */
  for (lib = loadedlibs; lib->func; lib++) {
    luaL_requiref(_state, lib->name, lib->func, 1);
    lua_pop(_state, 1);  /* remove lib */
  }
}

/**
 * @brief Register C function handlers to Lua interpreter
 * 
 * @param name Function name in Lua script
 * @param function C function handler
 */
void LuaWrapper::LW_RegisterFunc(const char *name, const lua_CFunction function) {
  lua_register(_state, name, function);
}

/**
 * @brief Execute a Lua script from filesystem, and optionally close the session
 * 
 * @param filename Filename on filesystem to execute Lua script
 * @param close_LVM Bool to close the LVM session (default: false)
 */
void LuaWrapper::LW_ExecuteFile(const char *filename, bool close_LVM) {
  if (luaL_dofile(_state, filename)) {
    Serial.printf("# lua error: %s\n", lua_tostring(_state, -1));
    lua_pop(_state, 1);
  }
  
  if (close_LVM == 1)
    lua_close(_state);
}

/**
 * @brief Garb Collector Full
 * 
 */
void LuaWrapper::LW_GarbCollectFull(){
  LuaC_gcfull(_state);
}