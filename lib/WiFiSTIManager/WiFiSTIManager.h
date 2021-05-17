#ifndef WiFiSTIManager_h
#define WiFiSTIManager_h

#include "WebServerAliases.h"
#include "WiFiAliases.h"
#include "ESPConfig.h"
#include "DebugMessage.h"

/**
 * @file WebStatic.h
 * @author Fabricio Tamusiunas (fabricio@tamusiunas.com)
 * @brief Common class to manage WiFi Access
 * @version 0.1
 * @date 2019-02-09
 * 
 * @copyright Copyright (c) 2019-2021
 * 
 */
class WiFiSTIManager
{
public:
  WiFiSTIManager(ESPConfig *espConfig);
  WiFiSTIManager();
  ~WiFiSTIManager();
  void start();
  void stop();
  bool isConnected();
private:
  ESPConfig *_espConfig;
  bool connect();
#ifdef ESP32
  static void WiFiEvent(WiFiEvent_t event);
#endif
  bool _hadBeenConnected = false;
  unsigned long connectTimeTry = 0;
  DebugMessage *debugMessage = new DebugMessage();
};

#endif
