/**
 * @file OTAHandler.h
 * @author Fabricio Tamusiunas (fabricio@tamusiunas.com)
 * @brief OTA (Over the Air) Management
 * @version 0.1
 * @date 2019-02-11
 * 
 * @copyright Copyright (c) 2019
 * 
 */

#ifndef OTAHandler_h
#define OTAHandler_h

#include "ESPConfig.h"
#include <ArduinoOTA.h>
#include "WiFiAliases.h"
#include "DebugMessage.h"

class OTAHandler
{
  public:
    OTAHandler();
    OTAHandler(ESPConfig *espConfig, String hostName);
    ~OTAHandler();
    void handle();
    void start();
    void setEspConfig(ESPConfig *espConfig);
    void setHostName(String hostName);

  private:
    ESPConfig *_espConfig;
    String _hostName;
    DebugMessage _debugMessage;
};


#endif
