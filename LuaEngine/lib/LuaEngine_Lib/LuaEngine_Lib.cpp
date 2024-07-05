#include "LuaEngine_Lib.h"

// Initialize static data members
Inp_Out *LuaEngine::IO;
uint16_t LuaEngine::maxBuffSize;
std::atomic<uint16_t> LuaEngine::LuaBuffID;
std::atomic<float> *LuaEngine::LuaBuffVar;
std::atomic<bool> LuaEngine::LuaNotifyWriteWait;
std::atomic<bool> LuaEngine::LuaScriptRestart;
std::atomic<uint8_t> LuaEngine::Lua_Shed_Stat;
std::atomic<uint8_t> LuaEngine::Action_CmdID;
std::atomic<float> LuaEngine::Action_CmdVAL;
std::atomic<int8_t> LuaEngine::ARS_Stat;
std::atomic<bool> LuaEngine::LuaNotifyReadWait;


/**
 * @brief Get current milliseconds
 * 
 * @param lua_state Pointer to Lua interpreter state
 * @return int Status for Lua interpreter
 */
uint8_t LuaEngine::LuaFunc_Millis(lua_State *lua_state) {
  lua_pushinteger(lua_state, millis());
  return 1;
}

/**
 * @brief Block Lua task for a specified duration
 * 
 * @param lua_state Pointer to Lua interpreter state
 * @return int Status for Lua interpreter
 */
uint8_t LuaEngine::LuaFunc_Delay(lua_State *lua_state) {
  int delay_ms = luaL_checkinteger(lua_state, 1);
  delay(delay_ms);
  return 0;
}

int LuaEngine::LuaFunc_Print(lua_State *L) {
  int n = lua_gettop(L); // Number of arguments
  int i;
  lua_getglobal(L, "tostring");
  for (i = 1; i <= n; i++) {
    const char *s;
    size_t l;
    lua_pushvalue(L, -1); // Function to be called
    lua_pushvalue(L, i); // Value to print
    lua_call(L, 1, 1);
    s = lua_tolstring(L, -1, &l); // Get result
    if (s == NULL)
      return luaL_error(L, "'tostring' must return a string to 'print'");
    if (i > 1) 
      Serial.write(" ");
    Serial.write(s);
    lua_pop(L, 1); // Pop result
  }
  Serial.println();
  return 0;
}

/**
 * @brief Read from the shared Lua buffer variables
 * 
 * @param lua_state Pointer to Lua interpreter state
 * @return int Status for Lua interpreter
 */
uint8_t LuaEngine::LuaFunc_Read(lua_State *lua_state) {
  LuaBuffID = luaL_checkinteger(lua_state, 1);
  if (LuaBuffID > 0 && LuaBuffID <= maxBuffSize) {
    lua_pushnumber(lua_state, LuaBuffVar[LuaBuffID - 1]);
    return 1;
  }
  else
    return 0;
}

/**
 * @brief Write to the shared Lua buffer variables, and wait until notified by another task
 * 
 * @param lua_state Pointer to Lua interpreter state
 * @return int Status for Lua interpreter
 */
uint8_t LuaEngine::LuaFunc_WriteWait(lua_State *lua_state) {
  LuaBuffID = luaL_checkinteger(lua_state, 1);
  if (LuaBuffID < 0 || LuaBuffID > maxBuffSize) {
    lua_pushinteger(lua_state, LUA_WRITE_WRONG_ID);
    return 1;
  }

  LuaBuffVar[LuaBuffID - 1] = luaL_checknumber(lua_state, 2);
  LuaNotifyWriteWait = 1; // Notify tasks that Lua task is waiting

  uint32_t notifyResp;
  if (xTaskNotifyWait(0, 0, &notifyResp, portMAX_DELAY) == pdTRUE) // Received notification
    lua_pushinteger(lua_state, notifyResp);
  else // No notification received
    lua_pushinteger(lua_state, LUA_WRITE_NO_NOTIFY);

  LuaNotifyWriteWait = 0; // Reset value

  return 1;
}

/**
 * @brief Write to the shared Lua buffer variables
 * 
 * @param lua_state Pointer to Lua interpreter state
 * @return int Status for Lua interpreter
 */
uint8_t LuaEngine::LuaFunc_WriteNoWait(lua_State *lua_state) {
  LuaBuffID = luaL_checkinteger(lua_state, 1);
  if (LuaBuffID > 0 && LuaBuffID <= maxBuffSize)
    LuaBuffVar[LuaBuffID - 1] = luaL_checknumber(lua_state, 2);

  return 0;
}

/**
 * @brief Check RPC command values in shared Lua buffer
 * 
 * @param lua_state Pointer to Lua interpreter state
 * @return int Status for Lua interpreter
 */
uint8_t LuaEngine::LuaFunc_RPCRead(lua_State *lua_state) {
  LuaBuffID = luaL_checkinteger(lua_state, 1);
  if (LuaBuffID > 0 && LuaBuffID <= maxBuffSize) {
    lua_pushnumber(lua_state, LuaBuffVar[LuaBuffID - 1]);
    return 1;
  }
  else
    return 0;
}

/**
 * @brief Verify date-time required with three overloads
 * 
 * @param lua_state Pointer to Lua interpreter state
 * @return int Status for Lua interpreter
 */
uint8_t LuaEngine::LuaFunc_TimeVerify(lua_State *lua_state) {
  // Decode parameters
  bool t = 0; // Verify time only
  bool toffset = 0; // Verify time with offset
  bool dt = 0; // Verify date and time

  if (lua_type(lua_state, 2) == LUA_TNUMBER)
    toffset = 1;
  else if (lua_type(lua_state, 2) == LUA_TSTRING)
    dt = 1;
  else
    t = 1;

  if (t) {
    const char *time = luaL_checkstring(lua_state, 1);
    if (time != NULL) {
      lua_pushboolean(lua_state, IO->VerifyTime(time));
      return 1;
    }
    else
      return 0;
  }

  else if (toffset) {
    const char *time = luaL_checkstring(lua_state, 1);
    int offset = luaL_checkinteger(lua_state, 2);
    if (time != NULL && offset >= 0) {
      lua_pushboolean(lua_state, IO->VerifyTime(time, offset));
      return 1;
    }
    else
      return 0;
  }

  else if (dt) {
    const char *date = luaL_checkstring(lua_state, 1);
    const char *time = luaL_checkstring(lua_state, 2);
    if (date != NULL && time != NULL) {
      lua_pushboolean(lua_state, IO->VerifyTime(date, time));
      return 1;
    }
    else
      return 0;
  }
  
  return 0;
}

/**
 * @brief Check whether a request to update the Lua task is active
 * 
 * @param lua_state Pointer to Lua interpreter state
 * @return int Status for Lua interpreter
 */
uint8_t LuaEngine::LunFunc_ScriptRestart(lua_State *lua_state) {
  if (LuaScriptRestart == 1)
    lua_pushboolean(lua_state, 1);
  else
    lua_pushboolean(lua_state, 0);
  
  return 1;
}

/**
 * @brief Update key in NVS
 * 
 * @param lua_state Pointer to Lua interpreter state
 * @return int Status for Lua interpreter
 */
uint8_t LuaEngine::LuaFunc_NVS_Incrm(lua_State *lua_state) {
  const char *nvs_key = luaL_checkstring(lua_state, 1); // Key in NVS to increment
  int val = luaL_checkinteger(lua_state, 2); // Interval in minutes to increment in NVS

  // Error handling
  if (val <= 0)
    return 0;
  
  // Update NVS
  Preferences preferences;
  preferences.begin(LUA_NVS_HEADER);

  unsigned long nvs_val = preferences.getULong(nvs_key, 0);
  nvs_val += val;
  preferences.putULong(nvs_key, nvs_val);

  preferences.end();

  Serial.printf("Written to %s: %lu\n", nvs_key, nvs_val);

  return 0;
}

/**
 * @brief Get parameter index of key with least value in NVS, with lower index devices being selected when equal value
 * 
 * @param lua_state Pointer to Lua interpreter state
 * @return int Status for Lua interpreter
 */
uint8_t LuaEngine::LuaFunc_NVS_GetMin(lua_State *lua_state) {
  unsigned int min_idx = 0; // Index of function parameter with least value in NVS
  unsigned long min = 0; // Least runtime stored on NVS

  Preferences preferences;
  preferences.begin(LUA_NVS_HEADER);
  
  // Read as many key as passed
  bool new_key = 0;
  for (unsigned int i = 1; lua_type(lua_state, i) == LUA_TSTRING; i++) {
    const char *nvs_key = luaL_checkstring(lua_state, i);

    if (preferences.isKey(nvs_key)) { // If key exists in NVS
      unsigned long nvs_val = preferences.getULong(nvs_key);

      if (nvs_val < min || i == 1) {
        min = nvs_val;
        min_idx = i;
      }
    }
    else { // If key does not exist in NVS
      preferences.putULong(nvs_key, 0);

      if (new_key == 0 && min > 0) {
        min = 0;
        min_idx = i;
        new_key = 1;
      }
    }
  }

  preferences.end();

  if (min_idx == 0)
    min_idx = 1; // Select first parameter as minimum when all values are 0

  lua_pushinteger(lua_state, min_idx);

  Serial.printf("Selected key: %u\n", min_idx);

  return 1;
}

/**
 * @brief Get value of the key stored in NVS
 * 
 * @param lua_state Pointer to Lua interpreter state
 * @return int Status for Lua interpreter
 */
uint8_t LuaEngine::LuaFunc_NVS_GetVal(lua_State *lua_state) {
  const char *nvs_key = luaL_checkstring(lua_state, 1); // Key in NVS to get value of

  // Read from NVS
  Preferences preferences;
  preferences.begin(LUA_NVS_HEADER);
  int nvs_val = preferences.getInt(nvs_key, 0);
  preferences.end();

  lua_pushinteger(lua_state, nvs_val);

  return 1;
}

/**
 * @brief Update the int value in NVS
 * 
 * @param lua_state Pointer to Lua interpreter state
 * @return uint8_t value to update
 */
uint8_t LuaEngine::LuaFunc_NVS_WriteInt(lua_State *lua_state){
  const char *nvs_key = luaL_checkstring(lua_state, 1); // Key in NVS to increment
  int val = luaL_checkinteger(lua_state, 2); // Value to Write
  
  // Update NVS
  Preferences preferences;
  preferences.begin(LUA_NVS_HEADER);

  preferences.putInt(nvs_key, val);

  preferences.end();

  Serial.printf("Written to %s: %d\n", nvs_key, val);

  return 1;

}


/**
 * @brief Check the shedule values
 * 
 * @param lua_state Pointer to Lua interpreter state
 * @return int Status for Lua interpreter
 */
uint8_t LuaEngine::LuaFunc_CheckShedule(lua_State *lua_state){
  //Serial.print(Lua_Shed_Stat);
  lua_pushinteger(lua_state, Lua_Shed_Stat.load()); // Sending the Shedule_Stat number 
  return(1);
}

/**
 * @brief Read the Action value of corrresponding ID
 * 
 * @param lua_state Pointer to Lua interpreter state
 * @return int int Status for Lua interpreter

int LuaEngine::LuaFunc_ReadActCmd(lua_State *lua_state){
  uint8_t Actcm = luaL_checkinteger(lua_state, 1);

  if(Actcm > 0){
    if(Actcm == Action_CmdID){
      lua_pushnumber(lua_state, Action_CmdVAL);
    }

    else{
      Serial.print("\nAct not match");
      lua_pushnumber(lua_state, -1);
    }
  }
  else{
    Serial.print("\nAct not match");
    lua_pushnumber(lua_state, -1);
  }
    
}
*/

/**
 * @brief Read the ACTCmd ID
 * 
 * @param lua_state Pointer to Lua interpreter state
 * @return int Status for Lua interpreter
 */
uint8_t LuaEngine::LuaFunc_ReadActCmdID(lua_State *lua_state){
  lua_pushnumber(lua_state, Action_CmdID);
  return 1;
}

/**
 * @brief Read the ACTCmd Value
 * 
 * @param lua_state Pointer to Lua interpreter state
 * @return int Status for Lua interpreter
 */
uint8_t LuaEngine::LuaFunc_ReadActCmdVal(lua_State *lua_state){
  lua_pushnumber(lua_state, Action_CmdVAL);
  return 1;
}

/**
 * @brief Action Cmd Reset
 * 
 * @param lua_state Pointer to Lua interpreter state
 * @return int Status for Lua interpreter
 */
uint8_t LuaEngine::LuaFunc_ActcmdReset(lua_State *lua_state){
  Action_CmdID = 0;
  Action_CmdVAL = -1;
  return 1;
}

/**
 * @brief Garbage Collection full 
 * 
 * @param lua_state Pointer to Lua interpreter state
 * @return uint8_t Status for Lua interpreter
 */
uint8_t LuaEngine::LuaFunc_GC_full(lua_State *lua_state){
  LuaC_gcfull(lua_state);
  return 1;
}

/**
 * @brief Lua Function ARS Stat
 * 
 * @param lua_state Pointer to Lua interpreter state
 * @return uint8_t Status for Lua interpreter
 */
uint8_t LuaEngine::LuaFunc_ARS_Stat(lua_State *lua_state){
  ARS_Stat = luaL_checknumber(lua_state, 1);
  return 1;
}


/**
 * @brief Read from the shared Lua buffer variables instantantly with wait
 * 
 * @param lua_state Pointer to Lua interpreter state
 * @return int int Status for Lua interpreter
 */
int LuaEngine::LuaFunc_ReadWait(lua_State *lua_state) {
  LuaBuffID = luaL_checkinteger(lua_state, 1);

  if (LuaBuffID < 0 || LuaBuffID > maxBuffSize) {
    Serial.print("\n Wrong Lua BufID1");
    lua_pushinteger(lua_state, LUA_WRITE_WRONG_ID);
    lua_pushinteger(lua_state, NO_DAT);
    return 2; // returning 2 values
  }

  else {
    LuaNotifyReadWait = 1;
    uint32_t notifyResp;

    if (xTaskNotifyWait(0, 0, &notifyResp, portMAX_DELAY) == pdTRUE){ // Received notification
      lua_pushinteger(lua_state, notifyResp);
      lua_pushinteger(lua_state, LuaBuffVar[LuaBuffID - 1]);
    }

    else{
      lua_pushinteger(lua_state, LUA_WRITE_NO_NOTIFY);
      lua_pushinteger(lua_state, NO_DAT);
    }

    LuaNotifyReadWait = 0;
    return 2; // returning 2 values
  }
}

/**
 * @brief Map Lua script functions to Arduino functions
 * 
 * @param luaWrap Object to Lua wrapper
 */
void LuaEngine::Lua_TaskMapFunc(LuaWrapper &LW) {
  LW.LW_RegisterFunc(Lua_Millis_FuncName, (const lua_CFunction) &LuaFunc_Millis);
  LW.LW_RegisterFunc(Lua_Delay_FuncName, (const lua_CFunction) &LuaFunc_Delay);
  LW.LW_RegisterFunc(Lua_Print_FuncName, (const lua_CFunction) &LuaFunc_Print);
  LW.LW_RegisterFunc(Lua_BuffRead_FuncName, (const lua_CFunction) &LuaFunc_Read);
  LW.LW_RegisterFunc(Lua_BuffWriteWait_FuncName, (const lua_CFunction) &LuaFunc_WriteWait);
  LW.LW_RegisterFunc(Lua_BuffWriteNoWait_FuncName, (const lua_CFunction) &LuaFunc_WriteNoWait);
  LW.LW_RegisterFunc(Lua_BuffRPC_FuncName, (const lua_CFunction) &LuaFunc_RPCRead);
  LW.LW_RegisterFunc(Lua_Time_FuncName, (const lua_CFunction) &LuaFunc_TimeVerify);
  LW.LW_RegisterFunc(Lua_ScriptRestart_FuncName, (const lua_CFunction) &LunFunc_ScriptRestart);
  LW.LW_RegisterFunc(Lua_NVSIncrm_FuncName, (const lua_CFunction) &LuaFunc_NVS_Incrm);
  LW.LW_RegisterFunc(Lua_NVSGetMin_FuncName, (const lua_CFunction) &LuaFunc_NVS_GetMin);
  LW.LW_RegisterFunc(Lua_NVSGetVal_FuncNAme, (const lua_CFunction) &LuaFunc_NVS_GetVal);
  LW.LW_RegisterFunc(Lua_NVSWriteInt_FuncNAme, (const lua_CFunction) &LuaFunc_NVS_WriteInt);
  LW.LW_RegisterFunc(Lua_CheckShedule, (const lua_CFunction) &LuaFunc_CheckShedule);
  LW.LW_RegisterFunc(Lua_ReadActCmdID, (const lua_CFunction) &LuaFunc_ReadActCmdID);
  LW.LW_RegisterFunc(Lua_ReadActCmdVal, (const lua_CFunction) &LuaFunc_ReadActCmdVal);
  LW.LW_RegisterFunc(Lua_ActCMDReset, (const lua_CFunction) &LuaFunc_ActcmdReset);
  LW.LW_RegisterFunc(Lua_GC_full, (const lua_CFunction) &LuaFunc_GC_full);
  LW.LW_RegisterFunc(Lua_ARSStat, (const lua_CFunction) &LuaFunc_ARS_Stat);
  LW.LW_RegisterFunc(Lua_BuffReadWait_FuncName, (const lua_CFunction) &LuaFunc_ReadWait);
}

/**
 * @brief Allocate the shared buffer for Lua variables and create the Lua task
 * 
 * @param LB_sz Number of elements to allocate
 */
void LuaEngine::Lua_TaskAndBuffInit(uint16_t LB_sz) {
  if (LE_ERC != NO_ERROR)
    return;

  LuaBuffVar = (std::atomic<float> *) calloc(LB_sz, sizeof(float));
    
  if (LuaBuffVar == NULL) {
    Serial.printf("Failed in dynamic allocation of Lua shared buffer\n");
    LE_ERC = BUFF_FAIL;
    maxBuffSize = 0;
    return;
  }

  maxBuffSize = LB_sz;

  // Reset all values to -1, to signify NO_DAT in Lua script
  for (int i = 0; i < maxBuffSize; i++)
    LuaBuffVar[i] = -1;

  BaseType_t xTaskStatus = xTaskCreate(&Lua_Task, "lua_task", LUA_STACK_SIZE, this, LUA_TASK_PRIORITY, &Lua_TaskHandle);
  if (xTaskStatus != pdPASS) {
    Serial.printf("Failed in creation of Lua task\n");
    LE_ERC = TASK_FAIL;
    maxBuffSize = 0;
  }
}

/**
 * @brief Lua task to handle the scripts
 * 
 * @param pvParameters User defined parameters
 */
void LuaEngine::Lua_Task(void *pvParameters) {
  LuaEngine *LE = (LuaEngine *) pvParameters; // Dereference the Lua engine object

  LuaWrapper LW; // Object to Lua wrapper

  while (1) {
    LW.LW_ResetLVM();
    LE->Lua_TaskMapFunc(LW);
    LW.LW_ExecuteFile(LF_Files_Path);
    LW.LW_ExecuteFile(LM_Files_Path, 1);

    #if LUA_CHECK_HIGH_WATER_MARK
    Serial.printf("Lua task stack free: %u\n", uxTaskGetStackHighWaterMark(NULL));
    #endif

    // Reset RPC trigger
    if (LE->LuaScriptRestart == 1)
      LE->LuaScriptRestart = 0;
    
    delay(5000);
  }
}

/**
 * @brief Lua & IO Sync function
 * 
 * @param LE Lua Engine Ref
 * @param _Io InputOutput Ref
 */
void LuaEngine::Lua_IO_Sync(LuaEngine &LE, Inp_Out &_Io){
  //Serial.printf("\n LShed : %d", _Io.Shedule_Stat);
  LuaEngine::Lua_Shed_Stat = _Io.Shedule_Stat;
}

/**
 * @brief Input Cmd variables
 * 
 * @param _CmdID Cmd ID
 * @param _CmdVal Cmd Val
 */
void LuaEngine::Input_CmdVariable(uint8_t _CmdID, float _CmdVal){
  Action_CmdID = _CmdID;
  Action_CmdVAL = _CmdVal;
}