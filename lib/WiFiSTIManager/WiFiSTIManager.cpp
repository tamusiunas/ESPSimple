#include "WiFiSTIManager.h"

WiFiSTIManager::WiFiSTIManager(ESPConfig *espConfig)
{
  _espConfig = espConfig;
}

WiFiSTIManager::~WiFiSTIManager()
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
  Serial.println("IPIp: " + IPIp.toString());
  Serial.println("IPMask: " + IPMask.toString());
  Serial.println("IPGw: " + IPGw.toString());
  Serial.println("IPDns: " + IPDns.toString());
  if (hasIP and hasMask and hasGw and hasDNS)
  {
    Serial.println("IPIp, IPGw, IPMask, IPDns");
    WiFi.config(IPIp, IPGw, IPMask, IPDns);
  }
  else
  if (hasIP and hasMask and hasGw)
  {
    Serial.println("IPIp, IPGw, IPMask");
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
      Serial.println("Not connected");
      WiFi.printDiag(Serial);
      return false;
    }
    else
    {
      _hadBeenConnected = true;
      Serial.println("Connected");
      WiFi.printDiag(Serial);
      Serial.println("IP: " + WiFi.localIP());
      return true;
    }
  }
  else
  {
    _hadBeenConnected = true;
    Serial.println("Connected");
    WiFi.printDiag(Serial);
    Serial.println("IP: " + WiFi.localIP());
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
    Serial.printf("[WiFi-event] event: %d\n", event);

    switch (event)
    {
        case SYSTEM_EVENT_WIFI_READY:
            Serial.println("WiFi interface ready");
            break;
        case SYSTEM_EVENT_SCAN_DONE:
            Serial.println("Completed scan for access points");
            break;
        case SYSTEM_EVENT_STA_START:
            Serial.println("WiFi client started");
            break;
        case SYSTEM_EVENT_STA_STOP:
            Serial.println("WiFi clients stopped");
            break;
        case SYSTEM_EVENT_STA_CONNECTED:
            Serial.println("Connected to access point");
            break;
        case SYSTEM_EVENT_STA_DISCONNECTED:
            Serial.println("Disconnected from WiFi access point");
            break;
        case SYSTEM_EVENT_STA_AUTHMODE_CHANGE:
            Serial.println("Authentication mode of access point has changed");
            break;
        case SYSTEM_EVENT_STA_GOT_IP:
            Serial.print("Obtained IP address: ");
            Serial.println(WiFi.localIP());
            break;
        case SYSTEM_EVENT_STA_LOST_IP:
            Serial.println("Lost IP address and IP address is reset to 0");
            break;
        case SYSTEM_EVENT_STA_WPS_ER_SUCCESS:
            Serial.println("WiFi Protected Setup (WPS): succeeded in enrollee mode");
            break;
        case SYSTEM_EVENT_STA_WPS_ER_FAILED:
            Serial.println("WiFi Protected Setup (WPS): failed in enrollee mode");
            break;
        case SYSTEM_EVENT_STA_WPS_ER_TIMEOUT:
            Serial.println("WiFi Protected Setup (WPS): timeout in enrollee mode");
            break;
        case SYSTEM_EVENT_STA_WPS_ER_PIN:
            Serial.println("WiFi Protected Setup (WPS): pin code in enrollee mode");
            break;
        case SYSTEM_EVENT_AP_START:
            Serial.println("WiFi access point started");
            break;
        case SYSTEM_EVENT_AP_STOP:
            Serial.println("WiFi access point  stopped");
            break;
        case SYSTEM_EVENT_AP_STACONNECTED:
            Serial.println("Client connected");
            break;
        case SYSTEM_EVENT_AP_STADISCONNECTED:
            Serial.println("Client disconnected");
            break;
        case SYSTEM_EVENT_AP_STAIPASSIGNED:
            Serial.println("Assigned IP address to client");
            break;
        case SYSTEM_EVENT_AP_PROBEREQRECVED:
            Serial.println("Received probe request");
            break;
        case SYSTEM_EVENT_GOT_IP6:
            Serial.println("IPv6 is preferred");
            break;
        case SYSTEM_EVENT_ETH_START:
            Serial.println("Ethernet started");
            break;
        case SYSTEM_EVENT_ETH_STOP:
            Serial.println("Ethernet stopped");
            break;
        case SYSTEM_EVENT_ETH_CONNECTED:
            Serial.println("Ethernet connected");
            break;
        case SYSTEM_EVENT_ETH_DISCONNECTED:
            Serial.println("Ethernet disconnected");
            break;
        case SYSTEM_EVENT_ETH_GOT_IP:
            Serial.println("Obtained IP address");
            break;
        case SYSTEM_EVENT_MAX:
            Serial.println("SYSTEM_EVENT_MAX");
            break;
    }
  }
  #endif
