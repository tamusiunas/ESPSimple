/**
 * @file SpiffsManager.h
 * @author Fabricio Tamusiunas (fabricio@tamusiunas.com)
 * @brief SPIFFS files manager
 * @version 0.1
 * @date 2019-02-11
 * 
 * @copyright Copyright (c) 2019
 * 
 */

#ifndef SpiffsManager_h
#define SpiffsManager_h

#include "DataStore.h"
#include <FS.h>
#ifdef ESP32
#include <SPIFFS.h>
#endif

class SpiffsManager
{
public:
  /**
   * @brief Construct a new Spiffs Manager object
   * 
   * @param fileName File Name
   * @param formatIfError If true format the File System in case of error
   * @param dataStore DataStore object with data to be saved or stored (in case of read)
   */
  SpiffsManager(const char *fileName, bool formatIfError, DataStore *dataStore);

  /**
   * @brief Destroy the Spiffs Manager object
   * 
   */
  ~SpiffsManager();

  /**
   * @brief Save DataStore object to file
   * 
   * @return int return -1 if error or 1 if success
   */
  int saveFile();

  /**
   * @brief read from file to DataStore object
   * 
   * @return int return -1 if error if File System, 0 if File Not Found or 1 if success
   */
  int readFile();

  /**
   * @brief Remove file from File System
   * 
   * @return int return -1 if error or 1 if success
   */
  int removeFile();

private:
  const char *_fileName;
  bool _formatIfError;
  DataStore *_dataStore;
};

#endif
