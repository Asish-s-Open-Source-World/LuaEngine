/**************************************************************
 *  Author : Asish Johney George
 *  Date : 06-07-2024
 *  Visit : https://github.com/asishjohney
 * 
 * LuaEngine Github Repo :
 *   https://github.com/Asish-s-Open-Source-World/LuaEngine.git

 **************************************************************
 * Example Details :
 *  To execute the Lua script written in "MainScript.lua" & "FuncScript.lua" in the SPIFFS
 * 
 * Instruction :
 *  1. Flash the "MainScript.lua" & "FuncScript.lua" in the SPIFFS.
 *  2. Use the partition which supports SPIFFS
 *  3. FuncScript.lua = Include the Lua functions
 *  4. MainScript.lua = Include the main script & function calls
 *
 *
 **************************************************************
*/

#include <Arduino.h>
#include <LuaEngine_Lib.h>
#include <SPIFFSConfig_Lib.h>

// Create instances of SPIFFS_Config and LuaEngine classes
SPIFFS_Config SP_CNF;
LuaEngine LE;

void setup() {

    Serial.begin(115200);

    SP_CNF.SPIFFS_begin(); // Initialize SPIFFS (SPI Flash File System)
    SP_CNF.ListDir("/"); // List all files in the root directory of SPIFFS

    LE.Lua_TaskAndBuffInit(1); // Initialize Lua task and buffer
    
    // Check if the Lua engine initialization was successful
    if (LE.LE_ERC != NO_ERROR)
        Serial.printf("\nFailed to initialize Lua engine: %u", LE.LE_ERC);  // If there was an error, print the error code
    
    else
        Serial.print("\nSuccessfully initialized Lua engine");  // If initialization was successful, print a success message
}

// Loop function
void loop() {
    
    Serial.print("\nIn loop");
    Serial.print("\nHeap=");   Serial.print(ESP.getFreeHeap()); // Print the current free heap memory
    Serial.println()
    delay(2000);
}