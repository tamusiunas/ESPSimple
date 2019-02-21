#ifdef ESP32
#include <WiFi.h>
#define WifiGetChipId() (uint32_t)ESP.getEfuseMac()
#endif
#ifdef ESP8266
#include <ESP8266WiFi.h>
#define WifiGetChipId() ESP.getChipId()
#endif
