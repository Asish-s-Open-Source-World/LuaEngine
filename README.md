# LuaEngine 🎉


![1720165041413](image/README/1720165041413.png)


**LuaEngine** is a powerful, lightweight, and flexible Arduino library designed to integrate the Lua scripting language with different platforms. This library enables you to run Lua scripts directly from SPIFFS and ensures efficient execution with built-in garbage collection. It is ideal for running independent light scripts using the Lua programming language.

## ✨ Features

- Currently Supported microcontroller platform : Esp32
- Lua Supported Version : 5.4.4
- Independent FreeRTOS task created for Lua Interpreter
- Executing the Lua Script from SPIFFS
- Added some Inbuilt Arduino functions of delay(), millis() etc.
- Added Garbage collector internal function.

## 📚 Table of Contents

- [Installation](#installation)
- [Basic Usage](#basic-usage)
- [API Reference](#api-reference)
- [Examples](#examples)
- [Contributing](#contributing)
- [License](#license)

## 🚀 Installation

### Using PlatformIO

Add the following to your `platformio.ini`:

```ini
lib_deps = 
    https://github.com/yourusername/LuaEngine.git
```

### Manual Installation

1. Clone or download this repository.
2. Copy the `LuaEngine` folder into your Arduino `libraries` directory.

## 🛠️ Basic Usage

### Setup

Include the LuaEngine library and initialize it in your project:

```cpp
#include <Arduino.h>
#include <LuaEngine.h>

LuaEngine luaEngine;

void setup() {
    Serial.begin(115200);
    luaEngine.begin();
    luaEngine.runScript("/example.lua");
}

void loop() {
    // Your main code
}
```

### Example Lua Script (`example.lua`)

```lua
print("Hello from Lua!")
```

## 📖 API Reference

### LuaEngine

#### Constructor & Destructor

- `LuaEngine()`: Initializes a new Lua engine instance.
- `~LuaEngine()`: Cleans up the Lua engine instance.

#### Methods

- `void begin()`: Initializes the SPIFFS file system.
- `void runScript(const char* filename)`: Executes a Lua script from the SPIFFS.
- `void gc()`: Runs the Lua garbage collector to free up memory.
- `void uploadFile(const char* path, const uint8_t* data, size_t size)`: Uploads a file to the SPIFFS.
- `void deleteFile(const char* path)`: Deletes a file from the SPIFFS.

## 💡 Examples

### Running a Lua Script

```cpp
void setup() {
    Serial.begin(115200);
    luaEngine.begin();
    luaEngine.runScript("/hello.lua");
}

void loop() {
    // Your main code
}
```

#### `hello.lua`

```lua
print("Hello from Lua!")
```

### Remote File Management

```cpp
void uploadExampleFile() {
    const char* content = "print('Uploaded via SPIFFS')";
    luaEngine.uploadFile("/upload.lua", (uint8_t*)content, strlen(content));
}

void setup() {
    Serial.begin(115200);
    luaEngine.begin();
    uploadExampleFile();
    luaEngine.runScript("/upload.lua");
}

void loop() {
    // Your main code
}
```

## 🤝 Contributing

We welcome contributions from the community! Whether it's bug reports, feature requests, or pull requests, your input is valuable.

1. Fork the repository.
2. Create a new branch (`git checkout -b feature-branch`).
3. Make your changes and commit them (`git commit -m 'Add new feature'`).
4. Push to the branch (`git push origin feature-branch`).
5. Open a Pull Request.

## 📜 License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.
