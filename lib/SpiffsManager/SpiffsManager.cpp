/**
 * @file SpiffsManager.cpp
 * @author Fabricio Tamusiunas (fabricio@tamusiunas.com)
 * @brief SPIFFS files manager
 * @version 0.1
 * @date 2019-02-11
 * 
 * @copyright Copyright (c) 2019
 * 
 */

#include "SpiffsManager.h"

SpiffsManager::SpiffsManager(const char *fileName, bool formatIfError, DataStore *dataStore)
{
  _fileName = fileName;
  _formatIfError = formatIfError;
  _dataStore = dataStore;
}

SpiffsManager::~SpiffsManager()
{

}

int SpiffsManager::saveFile()
{
  if(!SPIFFS.begin())
  {
    if (_formatIfError)
    {
      SPIFFS.format();
      if(!SPIFFS.begin())
      {
        Serial.println("SPIFFS Mount Failed");
        return -1;
      }
    }
  }
  const size_t bufferSize = JSON_OBJECT_SIZE(_dataStore->getDataParametersCount());
  DynamicJsonBuffer jsonBuffer(bufferSize);
  JsonObject& root = jsonBuffer.createObject();

  int parameter_count = _dataStore->getDataParametersCount();
  DataParameter** parameters = _dataStore->getParameters();

  for (int parameterPos = 0; parameterPos < parameter_count; parameterPos++)
  {
    if (parameters[parameterPos] != NULL)
    {
      root[parameters[parameterPos]->getField()] = parameters[parameterPos]->getValue();
    }
  }
  String fileBackupStr = String(String(_fileName) + String(".backup"));

  File configFile = SPIFFS.open(_fileName, "w");
  if (!configFile) {
    return -1;
  }
  root.printTo(configFile);
  configFile.close();

  configFile = SPIFFS.open(fileBackupStr.c_str(), "w");
  if (!configFile) {
    return -1;
  }
  root.printTo(configFile);
  configFile.close();

  SPIFFS.end();
  return 1;
}

int SpiffsManager::readFile()
{
  _debugMessage->debug("SpiffsManager - Reading config File");
  if(!SPIFFS.begin())
  {
    if (_formatIfError)
    {
      _debugMessage->debug("SpiffsManager - Formating SPIFFS");
      SPIFFS.format();
      if(!SPIFFS.begin())
      {
        _debugMessage->debug("SpiffsManager - SPIFFS Mount Failed");
        return -1;
      }
    }
  }
  char *spiffFileName = NULL;
  if (SPIFFS.exists(_fileName))
  {
    spiffFileName = strdup(_fileName);
  }
  else
  {
    String fileBackupStr = String(String(_fileName) + String(".backup"));
    if (SPIFFS.exists(fileBackupStr.c_str()))
    {
      spiffFileName = strdup(fileBackupStr.c_str());
    }
    else
    {
      return -1;
    }
  }
  File configFile = SPIFFS.open(spiffFileName, "r");
  int jsonCont = 0;
  if (configFile)
  {
    _debugMessage->debug("SpiffsManager - Config file used: " + String(spiffFileName));
    while(configFile.available()){
      char jsonChar = configFile.read();
      if (jsonChar == ',')
      {
        ++jsonCont;
      }
    }
    configFile.seek(0);
    size_t size = configFile.size();
    std::unique_ptr<char[]> buf(new char[size]);
    configFile.readBytes(buf.get(), size);
    const size_t bufferSize = JSON_OBJECT_SIZE(jsonCont);
    DynamicJsonBuffer jsonBuffer(bufferSize);
    JsonObject& root = jsonBuffer.parseObject(buf.get());
    if (root.success())
    {
      for (auto lineJson : root) {
        _dataStore->addParameter(new DataParameter(lineJson.key,lineJson.value.as<char*>()));
      }
    }
  }
  else
  {
    return 0;
  }

  return 1;
}

int SpiffsManager::removeFile()
{
  if(!SPIFFS.begin())
  {
    if (_formatIfError)
    {
      SPIFFS.format();
      if(!SPIFFS.begin())
      {
        _debugMessage->debug("SpiffsManager - SPIFFS Mount Failed");
        return -1;
      }
    }
  }
  String fileBackupStr = String(String(_fileName) + String(".backup"));
  if(SPIFFS.remove(_fileName))
  {
    if(SPIFFS.remove(fileBackupStr.c_str()))
    {
      return 1;
    }
  }
  return -1;
}
