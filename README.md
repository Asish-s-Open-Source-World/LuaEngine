# LuaEngine 🎉

![1720190162326](image/README/1720190162326.png)

**LuaEngine** is a powerful, lightweight, and flexible Arduino library designed to integrate the Lua scripting language with different platforms. This library enables you to run Lua scripts directly from SPIFFS and ensures efficient execution with built-in garbage collection. It is ideal for running independent light scripts using the Lua programming language.

## ✨ Features

- Currently Supported microcontroller platform : Esp32
- Lua Supported Version : 5.4.4
- Independent FreeRTOS task created for running Lua Interpreter
- Executing the Lua Script from SPIFFS
- Added some Inbuilt Arduino functions of delay(), millis() etc.
- Added Garbage collector internal function.

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

1. Flash the "MainScript.lua" & "FuncScript.lua" in the SPIFFS.
2. Use the partition which supports SPIFFS
3. FuncScript.lua = Include the Lua functions
4. MainScript.lua = Include the main script & function calls

### Lua Script

#### MainScript.lua

The MainScript.lua Include the main script & function calls

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

The FuncScript.lua Include the Lua functions.


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


### Setup

Include the LuaEngine library and initialize it in your project:

```cpp
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
```

### Example Lua Script (`example.lua`)

```lua
print("Hello from Lua!")
```

## 📖 API Reference



## 🤝 Contributing

We welcome contributions from the community! Whether it's bug reports, feature requests, or pull requests, your input is valuable.

1. Fork the repository.
2. Create a new branch (`git checkout -b feature-branch`).
3. Make your changes and commit them (`git commit -m 'Add new feature'`).
4. Push to the branch (`git push origin feature-branch`).
5. Open a Pull Request.

## 📜 License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.
