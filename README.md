# LuaEngine 🎉

![LuaEngine Logo](image/README/1720190162326.png)

**LuaEngine** is a powerful, lightweight, and flexible Arduino library designed to integrate the Lua scripting language with different platforms. This library enables you to run Lua scripts directly from SPIFFS and ensures efficient execution with built-in garbage collection. It is ideal for running independent light scripts using the Lua programming language.

## ✨ Features

- Currently Supported Microcontroller Platform: ESP32
- Supported Platform versions : espressif32 @6.3.1 & arduino-esp32 @2.0.9.
- Lua Supported Version: 5.4.4
- Independent FreeRTOS task created for running Lua Interpreter
- Execute Lua Scripts from SPIFFS
- Includes built-in Arduino functions such as `delay()`, `millis()`, etc.
- Integrated Garbage Collector function

## 📚 Table of Contents

- [Installation](#installation)
- [Basic Usage](#basic-usage)
- [API Reference](#api-reference)
- [Contributing](#contributing)
- [License](#license)

## 🚀 Installation

### Using PlatformIO

Add the following to your `platformio.ini`:

```ini
lib_deps = 
    https://github.com/Asish-s-Open-Source-World/LuaEngine.git
```

### Manual Installation

1. Clone or download this repository.
2. Copy the `LuaEngine` folder into your Arduino `libraries` directory.

## 🛠️ Basic Usage

### Instructions

1. Flash the `MainScript.lua` & `FuncScript.lua` to the SPIFFS.
2. Use a partition that supports SPIFFS.
3. `FuncScript.lua`: Contains the Lua functions.
4. `MainScript.lua`: Contains the main script and function calls.

### Lua Scripts

#### MainScript.lua

Includes the main script and function calls.

```lua
-- Initializations of equipment
do
    print("Lua Execution Started")
end

while true do
    print("Executing the scripts...")

    local n = 5
    print("Factorial of "..n.." : "..factorial(n))

    Grb_collect() -- Garbage collector

    delay(2000) -- delay of 2 seconds
end
```

#### FuncScript.lua

Includes the Lua functions.

```lua
-- Lua function to calculate the factorial of a number
function factorial(n)
    if n == 0 then
        return 1
    else
        return n * factorial(n - 1)
    end
end
```

#### Setup

Include the LuaEngine library and initialize it in your project:

```cpp
#include <Arduino.h>
#include <LuaEngine/LuaEngine_Lib.h>
#include <SPIFFSConfig/SPIFFSConfig_Lib.h>

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
        Serial.printf("\nFailed to initialize Lua engine: %u", LE.LE_ERC); // If there was an error, print the error code
    else
        Serial.print("\nSuccessfully initialized Lua engine"); // If initialization was successful, print a success message
}

// Loop function
void loop() {
    Serial.print("\nIn loop");
    Serial.print("\nHeap="); Serial.print(ESP.getFreeHeap()); // Print the current free heap memory
    delay(2000);
}
```

#### Outputs

> rst:0x1 (POWERON_RESET),boot:0x13 (SPI_FAST_FLASH_BOOT)
> configsip: 0, SPIWP:0xee
> clk_drv:0x00,q_drv:0x00,d_drv:0x00,cs0_drv:0x00,hd_drv:0x00,wp_drv:0x00
> mode:DIO, clock div:2
> load:0x3fff0030,len:1184
> load:0x40078000,len:13192
> load:0x40080400,len:3028
> entry 0x400805e4
> E (337) esp_core_dump_flash: Incorre�~@size of core dump image: 270577936
> SPIFFS mount successful
> Listing directory structure from: /
>         FILE: FuncScript.lua, SIZE: 176
>         FILE: MainScript.lua, SIZE: 297

> Successfully initialized Lua engine
> In loop
> Heap=320016Lua Execution Started
> Executing the scripts...
> Factorial of 5 : 120

> In loop
> Heap=308388Executing the scripts...
> Factorial of 5 : 120

> In loop
> Heap=308388Executing the scripts...
> Factorial of 5 : 120

## 📖 API Reference

(Include detailed API references here, if available.)

## 🤝 Contributing

We welcome contributions from the community! Whether it's bug reports, feature requests, or pull requests, your input is valuable.

1. Fork the repository.
2. Create a new branch.
3. Make your changes and commit them .
4. Push to the branch.
5. Open a Pull Request.

## 📜 License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.
