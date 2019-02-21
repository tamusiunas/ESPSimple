#ifndef WiFiSTIManager_h
#define WiFiSTIManager_h

#include "WebServerAliases.h"
#include "WiFiAliases.h"
#include "ESPConfig.h"

class WiFiSTIManager
{
public:
  WiFiSTIManager(ESPConfig *espConfig);
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
};

#endif
