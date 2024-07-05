#ifndef LuaEngine_Lib_h
#define LuaEngine_Lib_h


#include <Arduino.h>
#include <atomic>
#include "LuaWrapper/LuaWrapper.h"
#include <ArduinoJson.h>
#include <Preferences.h>
#include "SPIFFS.h"

// Lua script functions mapping names

// Internal Arduino functions
#define Lua_Millis_FuncName "millis"
#define Lua_Delay_FuncName "delay"
#define Lua_Print_FuncName "print"

// Additional functions
#define Lua_ScriptRestart_FuncName "Script_Restart"
#define Lua_GC_full "Grb_collect" 
#define Lua_NVSGetVal_FuncNAme "NVS_GetVal"
#define Lua_NVSWriteInt_FuncNAme "NVS_WriteInt"

/*
#define Lua_Time_FuncName "Time_Trig"
#define Lua_BuffRead_FuncName "Buff_Read"
#define Lua_BuffWriteWait_FuncName "Buff_Write_Wait"
#define Lua_BuffWriteNoWait_FuncName "Buff_Write_NoWait"
#define Lua_BuffRPC_FuncName "RPC_Cmd"
#define Lua_NVSIncrm_FuncName "NVS_Incrm"
#define Lua_NVSGetMin_FuncName "NVS_MinSel" 
#define Lua_CheckShedule "Check_Shedule"
#define Lua_ReadActCmdID "Read_ActCmdID"
#define Lua_ReadActCmdVal "Read_ActCmdVal"
#define Lua_ActCMDReset "ActCMD_Reset"
#define Lua_ARSStat "ARS_Stat"
#define Lua_BuffReadWait_FuncName "Buff_Read_Wait"
*/

//Lua Function names
#define LF_Filename "/FuncScript.lua"
#define LM_Filename "/MainScript.lua"

#define LF_Files_Path "/spiffs" LF_Filename
#define LM_Files_Path "/spiffs" LM_Filename


// Lua engine error codes
#define NO_ERROR 0 // No error
#define BUFF_FAIL 1 // Failed to allocate the buffer
#define TASK_FAIL 2 // Failed to create Lua task
#define SCRIPT_RELOAD_FAIL 3 // Failed to reload Lua script
#define NO_DAT -1

// Lua script write error codes
#define LUA_WRITE_NO_NOTIFY -1 // Timeout without notfication from any task
#define LUA_WRITE_WRONG_ID -2 // Given Lua buffer ID is out of range

// Lua script NVS parameters
#define LUA_NVS_HEADER "LUA_NVS"

// Task parameters
#define LUA_STACK_SIZE 5500 // Stack allocation size for Lua task
#define LUA_TASK_PRIORITY 1 // Priority level of Lua task
#define LUA_CHECK_HIGH_WATER_MARK 1 // Display free stack size of Lua task

/**
 * @brief Handle Lua task and functionality
 * 
 */
class LuaEngine {
  private:

// Internal Arduino functions
  static uint8_t LuaFunc_Millis(lua_State *lua_state);
  static uint8_t LuaFunc_Delay(lua_State *lua_state);
  static int LuaFunc_Print(lua_State *lua_state);
  
// Additional functions
  static uint8_t LunFunc_ScriptRestart(lua_State *lua_state);
  static uint8_t LuaFunc_GC_full(lua_State *lua_state);
  static uint8_t LuaFunc_NVS_WriteInt(lua_State *lua_state);
  static uint8_t LuaFunc_NVS_GetVal(lua_State *lua_state);

  void Lua_TaskMapFunc(LuaWrapper &LW);

/*
  static uint8_t LuaFunc_Read(lua_State *lua_state);
  static uint8_t LuaFunc_WriteWait(lua_State *lua_state);
  static uint8_t LuaFunc_WriteNoWait(lua_State *lua_state);
  static uint8_t LuaFunc_RPCRead(lua_State *lua_state);
  static uint8_t LuaFunc_TimeVerify(lua_State *lua_state);
  static uint8_t LuaFunc_NVS_Incrm(lua_State *lua_state);
  static uint8_t LuaFunc_NVS_GetMin(lua_State *lua_state);
  
  static uint8_t LuaFunc_CheckShedule(lua_State *lua_state);
  static uint8_t LuaFunc_ReadActCmdID(lua_State *lua_state);
  static uint8_t LuaFunc_ReadActCmdVal(lua_State *lua_state);
  static uint8_t LuaFunc_ActcmdReset(lua_State *lua_state);
  static uint8_t LuaFunc_ARS_Stat(lua_State *lua_state);
  static int LuaFunc_ReadWait(lua_State *lua_state);
*/

  public:

  TaskHandle_t Lua_TaskHandle; // Pointer to handler of Lua task

  uint8_t LE_ERC; // Lua engine error code

  static uint16_t maxBuffSize; // Maximum number of elements in Lua buffer
//  static std::atomic<uint16_t> LuaBuffID; // Shared Lua buffer variable ID
  static std::atomic<float> *LuaBuffVar; // Pointer to shared Lua buffer variables
  static std::atomic<bool> LuaNotifyWriteWait; // Boolean to notify other tasks that Lua task is blocked till write request is completed

  static std::atomic<bool> LuaScriptRestart; // Boolean to check whether to restart the Lua script
  static std::atomic<uint8_t> Lua_Shed_Stat;

//  static std::atomic<uint8_t> Action_CmdID; // Action Command side 
//  static std::atomic<float> Action_CmdVAL;
//  static std::atomic<int8_t> ARS_Stat; // ARS Command

  static std::atomic<bool> LuaNotifyReadWait; // Boolean to notify other tasks that Lua task is blocked till Read request is completed
  
  /**
   * @brief Construct a new Lua Engine object
   * 
   */
  LuaEngine() {
    LE_ERC = NO_ERROR;
    maxBuffSize = 0;
    Lua_Shed_Stat = 0;
//    Action_CmdID = 0;
//    Action_CmdVAL = -1;
    LuaNotifyReadWait = 0;
//    ARS_Stat = -1;
  }

  void Lua_TaskAndBuffInit(uint16_t LB_sz);
  static void Lua_Task(void *pvParameters);
  
  //void Lua_IO_Sync(LuaEngine &LE, Inp_Out &_Io);
  void Lua_IO_Sync(LuaEngine &LE);
//  void Input_CmdVariable(uint8_t _CmdID, float _CmdVal);

};

#endif