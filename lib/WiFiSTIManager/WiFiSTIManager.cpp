#include "WiFiSTIManager.h"

WiFiSTIManager::WiFiSTIManager(ESPConfig *espConfig)
{
  _espConfig = espConfig;
}

WiFiSTIManager::~WiFiSTIManager()
{

}

WiFiSTIManager::WiFiSTIManager()
{

}

void WiFiSTIManager::start()
{

#ifdef ESP32
  WiFi.onEvent(WiFiEvent);
#endif
  if (WiFi.status() != WL_CONNECTED)
  {
    WiFiSTIManager::connect();
  }
}

void WiFiSTIManager::stop()
{
#ifdef ESP32
  WiFi.removeEvent(WiFiEvent);
#endif
  WiFi.disconnect(true);
}

bool WiFiSTIManager::connect()
{
  WiFi.mode(WIFI_STA);
  WiFi.enableAP(false);
  WiFi.enableSTA(true);
  bool hasIP = false;
  bool hasGw = false;
  bool hasMask = false;
  bool hasDNS = false;
  IPAddress IPIp;
  IPAddress IPGw;
  IPAddress IPMask;
  IPAddress IPDns;
  if (IPIp.fromString(_espConfig->getDataStore()->getValue("ip_address")) and (IPIp.toString() != "0.0.0.0"))
  {
    hasIP=true;
  }
  if (IPMask.fromString(_espConfig->getDataStore()->getValue("ip_mask")) and (IPMask.toString() != "0.0.0.0"))
  {
    hasMask=true;
  }
  if (IPGw.fromString(_espConfig->getDataStore()->getValue("ip_gateway")) and (IPGw.toString() != "0.0.0.0"))
  {
    hasGw=true;
  }
  if (IPDns.fromString(_espConfig->getDataStore()->getValue("ip_dns")) and (IPDns.toString() != "0.0.0.0"))
  {
    hasDNS=true;
  }
  debugMessage->debug("WiFiSTIManager - Ip: " + IPIp.toString());
  debugMessage->debug("WiFiSTIManager - Mask: " + IPMask.toString());
  debugMessage->debug("WiFiSTIManager - Gw: " + IPGw.toString());
  debugMessage->debug("WiFiSTIManager - Dns: " + IPDns.toString());
  if (hasIP and hasMask and hasGw and hasDNS)
  {
    //debugMessage->debug("IPIp, IPGw, IPMask, IPDns");
    WiFi.config(IPIp, IPGw, IPMask, IPDns);
  }
  else
  if (hasIP and hasMask and hasGw)
  {
    //debugMessage->debug("IPIp, IPGw, IPMask");
    WiFi.config(IPIp, IPGw, IPMask);
  }
  WiFi.persistent(true);
  String wifiName = "";
  String wifiPassword = _espConfig->getDataStore()->getValue("access_point_password");

  if (strcmp(_espConfig->getDataStore()->getValue("access_point_manual"),"") != 0)
  {
    wifiName = _espConfig->getDataStore()->getValue("access_point_manual");
  }
  else
  if (strcmp(_espConfig->getDataStore()->getValue("access_point_select"),"") != 0)
  {
    wifiName = _espConfig->getDataStore()->getValue("access_point_select");
  }
  if (WiFi.status() != WL_CONNECTED)
  {
    WiFi.begin(wifiName.c_str(),wifiPassword.c_str());
    if (WiFi.status() != WL_CONNECTED)
    {
      debugMessage->debug("WiFiSTIManager - Wifi is not connected");
      // WiFi.printDiag(Serial);
      return false;
    }
    else
    {
      _hadBeenConnected = true;
      debugMessage->debug("WiFiSTIManager - Wifi is connected");
      //WiFi.printDiag(Serial);
      debugMessage->debug("WiFiSTIManager - IP: " + WiFi.localIP());
      return true;
    }
  }
  else
  {
    _hadBeenConnected = true;
    debugMessage->debug("Connected");
    WiFi.printDiag(Serial);
    debugMessage->debug("WiFiSTIManager - IP: " + WiFi.localIP());
    return true;
  }
}

bool WiFiSTIManager::isConnected()
{
  if (WiFi.status() != WL_CONNECTED)
  {
    return false;
  }
  else
  {
    return true;
  }
}

#ifdef ESP32
void WiFiSTIManager::WiFiEvent(WiFiEvent_t event)
{
    DebugMessage debugMessageLocal = DebugMessage();
    debugMessageLocal.debug("WiFiSTIManager - Got an Wifi event: " + String(event));
    IPAddress ipAddress; 
    switch (event)
    {
        case SYSTEM_EVENT_WIFI_READY:
            debugMessageLocal.debug("WiFiSTIManager - Event - WiFi interface ready");
            break;
        case SYSTEM_EVENT_SCAN_DONE:
            debugMessageLocal.debug("WiFiSTIManager - Event - Completed scan for access points");
            break;
        case SYSTEM_EVENT_STA_START:
            debugMessageLocal.debug("WiFiSTIManager - Event - WiFi client started");
            break;
        case SYSTEM_EVENT_STA_STOP:
            debugMessageLocal.debug("WiFiSTIManager - Event - WiFi clients stopped");
            break;
        case SYSTEM_EVENT_STA_CONNECTED:
            debugMessageLocal.debug("WiFiSTIManager - Event - Connected to access point");
            break;
        case SYSTEM_EVENT_STA_DISCONNECTED:
            debugMessageLocal.debug("WiFiSTIManager - Event - Disconnected from WiFi access point");
            break;
        case SYSTEM_EVENT_STA_AUTHMODE_CHANGE:
            debugMessageLocal.debug("WiFiSTIManager - Event - Authentication mode of access point has changed");
            break;
        case SYSTEM_EVENT_STA_GOT_IP:
            ipAddress = IPAddress(WiFi.localIP());
            debugMessageLocal.debug("WiFiSTIManager - Event - Obtained IP address: " + ipAddress.toString());
            break;
        case SYSTEM_EVENT_STA_LOST_IP:
            debugMessageLocal.debug("WiFiSTIManager - Event - Lost IP address and IP address is reset to 0");
            break;
        case SYSTEM_EVENT_STA_WPS_ER_SUCCESS:
            debugMessageLocal.debug("WiFiSTIManager - Event - WiFi Protected Setup (WPS): succeeded in enrollee mode");
            break;
        case SYSTEM_EVENT_STA_WPS_ER_FAILED:
            debugMessageLocal.debug("WiFiSTIManager - Event - WiFi Protected Setup (WPS): failed in enrollee mode");
            break;
        case SYSTEM_EVENT_STA_WPS_ER_TIMEOUT:
            debugMessageLocal.debug("WiFiSTIManager - Event - WiFi Protected Setup (WPS): timeout in enrollee mode");
            break;
        case SYSTEM_EVENT_STA_WPS_ER_PIN:
            debugMessageLocal.debug("WiFiSTIManager - Event - WiFi Protected Setup (WPS): pin code in enrollee mode");
            break;
        case SYSTEM_EVENT_AP_START:
            debugMessageLocal.debug("WiFiSTIManager - Event - WiFi access point started");
            break;
        case SYSTEM_EVENT_AP_STOP:
            debugMessageLocal.debug("WiFiSTIManager - Event - WiFi access point  stopped");
            break;
        case SYSTEM_EVENT_AP_STACONNECTED:
            debugMessageLocal.debug("WiFiSTIManager - Event - Client connected");
            break;
        case SYSTEM_EVENT_AP_STADISCONNECTED:
            debugMessageLocal.debug("WiFiSTIManager - Event - Client disconnected");
            break;
        case SYSTEM_EVENT_AP_STAIPASSIGNED:
            debugMessageLocal.debug("WiFiSTIManager - Event - Assigned IP address to client");
            break;
        case SYSTEM_EVENT_AP_PROBEREQRECVED:
            debugMessageLocal.debug("WiFiSTIManager - Event - Received probe request");
            break;
        case SYSTEM_EVENT_GOT_IP6:
            debugMessageLocal.debug("WiFiSTIManager - Event - IPv6 is preferred");
            break;
        case SYSTEM_EVENT_ETH_START:
            debugMessageLocal.debug("WiFiSTIManager - Event - Ethernet started");
            break;
        case SYSTEM_EVENT_ETH_STOP:
            debugMessageLocal.debug("WiFiSTIManager - Event - Ethernet stopped");
            break;
        case SYSTEM_EVENT_ETH_CONNECTED:
            debugMessageLocal.debug("WiFiSTIManager - Event - Ethernet connected");
            break;
        case SYSTEM_EVENT_ETH_DISCONNECTED:
            debugMessageLocal.debug("WiFiSTIManager - Event - Ethernet disconnected");
            break;
        case SYSTEM_EVENT_ETH_GOT_IP:
            debugMessageLocal.debug("WiFiSTIManager - Event - Obtained IP address");
            break;
        case SYSTEM_EVENT_MAX:
            debugMessageLocal.debug("WiFiSTIManager - Event - SYSTEM_EVENT_MAX");
            break;
    }
  }
  #endif
