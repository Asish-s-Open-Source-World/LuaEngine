#ifndef SPIFFS_Config_Lib_h
#define SPIFFS_Config_Lib_h

#include <Arduino.h>
#include <EEPROM.h>
#include "FS.h"
#include "SPIFFS.h"


#define FORMAT_SPIFFS_IF_FAILED true

/**
 * @brief Manage files in SPIFFS partition
 * 
 */
class SPIFFS_Config {
  public:
  
  void SPIFFS_begin();
  void ListDir(const char *dirname);
  void ReadFile(const char *path);
  void WriteFile(const char *path, const char *message);
  void RenameFile(const char *path1, const char* path2);
  void DeleteFile(const char *path);
  void AppendFile(const char *path, const char *message);
  void Clear_File(const char* fn); // clearing the file - File name
};

#endif
