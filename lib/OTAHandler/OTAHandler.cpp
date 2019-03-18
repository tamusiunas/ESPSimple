/**
 * @file OTAHandler.cpp
 * @author Fabricio Tamusiunas (fabricio@tamusiunas.com)
 * @brief OTA (Over the Air) Management
 * @version 0.1
 * @date 2019-02-11
 * 
 * @copyright Copyright (c) 2019
 * 
 */


#include "OTAHandler.h"

OTAHandler::OTAHandler(DebugMessage *debugMessage)
{
  _debugMessage = debugMessage;
}

OTAHandler::OTAHandler(ESPConfig *espConfig, String hostName, DebugMessage *debugMessage)
{
  _espConfig = espConfig;
  _hostName = hostName;
  _debugMessage = debugMessage;
}

OTAHandler::~OTAHandler()
{
  _hostName = String(String("IOT-") + String(WifiGetChipId()));
}

void OTAHandler::start()
{
  if (strcmp(_espConfig->getDataStore()->getValue("enable_ota"),"yes") == 0)
  {
    ArduinoOTA.setHostname(_hostName.c_str());
    ArduinoOTA.setPassword(_espConfig->getDataStore()->getValue("ota_password"));
    ArduinoOTA.begin();
    _debugMessage->debug("OTA is enabled");
  }
  else
  {
    _debugMessage->debug("OTA is disabled");
  }
}

void OTAHandler::setEspConfig(ESPConfig *espConfig)
{
  _espConfig = espConfig;
}

void OTAHandler::setHostName(String hostName)
{
  _hostName = hostName;
}

void OTAHandler::handle()
{
  ArduinoOTA.handle();
}
