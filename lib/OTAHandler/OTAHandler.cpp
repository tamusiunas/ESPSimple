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

OTAHandler::OTAHandler()
{

}

OTAHandler::OTAHandler(ESPConfig *espConfig, String hostName)
{
  _espConfig = espConfig;
  _hostName = hostName;
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
    Serial.println("OTA enabled");
  }
  else
  {
    Serial.println("OTA disabled");
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
