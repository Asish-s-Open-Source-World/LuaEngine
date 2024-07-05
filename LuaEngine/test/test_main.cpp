#include <Arduino.h>
#include "LuaEngine_Lib.h"
#include <gtest/gtest.h>
#include "SPIFFSConfig_Lib.h"

void LuaEng_test(void){

  SPIFFS_Config SP_CNF;
  LuaEngine LE;

  SP_CNF.SPIFFS_begin();
  SP_CNF.ListDir("/");

  LE.Lua_TaskAndBuffInit(1);
    
  if (LE.LE_ERC != NO_ERROR)
    Serial.printf("\nFailed to initialize Lua engine: %u", LE.LE_ERC);
  
  else
    Serial.print("\nSuccessfully initialized Lua engine");
  
  while(1){
    Serial.print("\nIn loop");
    Serial.print("\nHeap=");   Serial.print(ESP.getFreeHeap());
    delay(2000);
  }
}

// Setup function, runs once before all tests
void setup() {
    Serial.begin(115200);
    // Initialize the GoogleTest framework

    ::testing::InitGoogleTest();
    //RUN_ALL_TESTS();

    LuaEng_test();
    //URL_parser();
}

// Loop function, required but not used for unit tests
void loop() {
    // Leave this empty for unit tests
}