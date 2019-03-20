/**
 * @file WebConfig.cpp
 * @author Fabricio Tamusiunas (fabricio@tamusiunas.com)
 * @brief Web config management
 * @version 0.1
 * @date 2019-02-09
 * 
 * @copyright Copyright (c) 2019
 * 
 */

#include "WebConfig.h"

WebConfig::WebConfig(ESPConfig *espConfig, const char *hostName, SpiffsManager *spiffsManager)
{
  _espConfig = espConfig;
  _hostName = String(hostName);
  _chipId=String(WifiGetChipId());
  _spiffsManager = spiffsManager;
}

WebConfig::WebConfig(ESPConfig *espConfig, SpiffsManager *spiffsManager)
{
  _espConfig = espConfig;
  _chipId=String(WifiGetChipId());

  _hostName = String(String("IOT-") + String(_chipId));
  _spiffsManager = spiffsManager;
  scanWifi();
}

WebConfig::~WebConfig()
{
  //WiFi.softAPdisconnect(true);
  //WiFi.disconnect(true,false);
}

void WebConfig::scanWifi()
{
  boolean hasSelected = false;
  String configFileWifiSelectedStr = _espConfig->getDataStore()->getValue("access_point_select");
  _wifiListHtmlOptions = "<option value=\"none\">None</option>";
  int wifiTotalFound = WiFi.scanNetworks();
  for (int cont = 0 ; cont < wifiTotalFound ; cont++)
  {
    String wifiSsid = String(WiFi.SSID(cont));
    String wifiStrenght = String(WiFi.RSSI(cont));
    if ((configFileWifiSelectedStr == wifiSsid) and (!hasSelected))
    {
      hasSelected = true;
      _wifiListHtmlOptions += String("<option value=\"" + wifiSsid + "\" selected>" + wifiSsid + " (" + wifiStrenght + "%) </option>");
    }
    else
    {
      _wifiListHtmlOptions += String("<option value=\"" + wifiSsid + "\">" + wifiSsid + " (" + wifiStrenght + "%) </option>");
    }
  }
}

bool WebConfig::startSsid(uint16_t httpPort, const char *ipAddress, const char *ssid, const char *password)
{
  // This must be migrated to GpioManager
  int gpio = String(_espConfig->getDataStore()->getValue("web_config_indicating")).toInt();
  String gpioStatus = String(_espConfig->getDataStore()->getValue("web_config_indicating_status"));
  pinMode(gpio, OUTPUT);
  if (gpioStatus == "on")
  {
    digitalWrite(gpio,HIGH);
  }
  else
  {
    digitalWrite(gpio,LOW);
  }
  // end of the block to be migrated
  
  _ipAddress.fromString(ipAddress);

  WiFi.mode(WIFI_AP);
  WiFi.enableSTA(false);
  WiFi.enableAP(true);
  delay(500);
  WiFi.softAPConfig(_ipAddress, _ipAddress, IPAddress(255, 255, 255, 0));
  WifiSetHostname(_hostName.c_str());
  if ((ssid != NULL) and (password != NULL))
  {
    WiFi.softAP(ssid,password,5,false,4);
  }
  else
  {
    WiFi.softAP(_hostName.c_str(),NULL,5,false,4);
  }
  _dnsServer = new DNSServer();
  _dnsServer->start(DNS_PORT, "*", _ipAddress);
  _dnsServer->setErrorReplyCode(DNSReplyCode::NoError);

  _webServer = new WebServerLocal(httpPort);
  _webServer->begin();
  WiFi.printDiag(Serial);
  handlePages();

  return true;
}

bool WebConfig::stopSsid()
{
  return true;
}

void WebConfig::processClient()
{
  isConfig = true;
  while (isConfig)
  {
    _dnsServer->processNextRequest();
    _webServer->handleClient();
  }
}
