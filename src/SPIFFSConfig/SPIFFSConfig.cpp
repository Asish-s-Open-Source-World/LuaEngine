#include "SPIFFSConfig.h"

/**
 * @brief Open a file from SPIFFS partition
 * 
 * @param filename Filename to open
 */
void SPIFFS_Config::SPIFFS_begin() {
  if (!SPIFFS.begin(FORMAT_SPIFFS_IF_FAILED)) {
    Serial.printf("SPIFFS mount failed\n");
    return;
  }
  else
    Serial.printf("SPIFFS mount successful\n");
}

/**
 * @brief List full directory sturcture and file sizes from a starting directory
 * 
 * @param dirname Directory name to list all directories and files
 */
void SPIFFS_Config::ListDir(const char *dirname) {
  Serial.printf("Listing directory structure from: %s\n", dirname);

  File root = SPIFFS.open(dirname);
  if (!root) {
    Serial.printf("Failed to open directory: %s\n", dirname);
    return;
  }  
  if (!root.isDirectory()) {
    Serial.printf("Not a valid directory: %s\n", dirname);
    return;
  }

  File file = root.openNextFile();

  while (file)  {
    Serial.printf("\tFILE: %s, SIZE: %u\n", file.name(), file.size());
    file = root.openNextFile();
  }

  file.close();
}

/**
 * @brief Read a file and display it on the Serial monitor
 * 
 * @param path Path of file to read
 */
void SPIFFS_Config::ReadFile(const char *path) {
  Serial.printf("Reading from file: %s\n", path);

  File file = SPIFFS.open(path);
  if (!file || file.isDirectory()) {
    Serial.printf("Failed to open file: %s\n", path);
    return;
  }

  Serial.printf("Read from file:\n");
  while (file.available())
    Serial.write(file.read());

  file.close();
}

/**
 * @brief Write to a file
 * 
 * @param path Path of file to write
 * @param message Buffer of data to write
 */
void SPIFFS_Config::WriteFile(const char *path, const char *message) {
  Serial.printf("Writing to file: %s\n", path);

  File file = SPIFFS.open(path, FILE_WRITE);
  if (!file) {
    Serial.printf("Failed to open file for writing\n");
    return;
  }

  if (file.print(message))
    Serial.printf("Successfully written to file: %s\n", path);
  else 
    Serial.printf("Failed to write to file: %s\n", path);

  file.close();
}

/**
 * @brief Append data to a file
 * 
 * @param path Path of file to append
 * @param message Buffer of data to append
 */
void SPIFFS_Config::AppendFile(const char *path, const char *message) {
  Serial.printf("Appending to file: %s\n", path);

  File file = SPIFFS.open(path, FILE_APPEND);
  if (!file) {
    Serial.printf("Failed to open file for appending\n");
    return;
  }

  if (file.print(message))
    Serial.printf("Successfully appended to file: %s\n", path);
  else 
    Serial.printf("Failed to append to file: %s\n", path);

  file.close();
}

/**
 * @brief Rename a file
 * 
 * @param path1 Original name of file with path
 * @param path2 New name of file with path
 */
void SPIFFS_Config::RenameFile(const char *path1, const char *path2) {
  Serial.printf("Renaming file %s to %s\n", path1, path2);

  if (SPIFFS.rename(path1, path2)) 
    Serial.printf("Successfully renamed file to: %s\n", path2);
  else
    Serial.printf("Failed in renaming file: %s\n", path1);
}

/**
 * @brief Delete a file
 * 
 * @param path Path of file to delete
 */
void SPIFFS_Config::DeleteFile(const char *path) {
  Serial.printf("Deleting file: %s\n", path);

  if (SPIFFS.remove(path))
    Serial.printf("Successfully deleted file: %s\n", path);
  else 
    Serial.printf("Failed in deleting file: %s\n", path);
}

void SPIFFS_Config::Clear_File(const char* fn) // clearing the buffer database
{
  if (SPIFFS.exists(fn)) //checking the db.logfile exists
  {
    Serial.printf("\n %s exist and started clearing...", fn);
    
    SPIFFS.remove(fn); // removing the db.log

    if(SPIFFS.exists(fn)) // checking the existance
    {
        Serial.print("\n E1: not removed");
    } 
  
    else
    {
      File dataFile = SPIFFS.open(fn, FILE_WRITE); // File creation
      dataFile.close();  
    
      if(SPIFFS.exists(fn)) // checking the existance
      {
        Serial.printf("\n %s successfully cleared.", fn);
      }

      else
      {
        Serial.print("\n E2: not removed");
      }       
    }
  }
  
  else
  {
    Serial.println();
    Serial.print(fn);
    Serial.print(" doesn't exists.");
  }
}