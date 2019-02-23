/**
 * @file MqttManagerIn.h
 * @author Fabricio Tamusiunas (fabricio@tamusiunas.com)
 * @brief Manager for MQTT incoming messages
 * @version 0.1
 * @date 2019-02-19
 * 
 * @copyright Copyright (c) 2019
 * 
 */
#ifndef MqttManagerIn_h
#define MqttManagerIn_h

#include <PubSubClient.h>
#include "GpioManager.h"
#include "ESPConfig.h"
#include "WiFiAliases.h"
#include "JsonManager.h"
#include "ArduinoJson.h"

//#include "JsonManager.h"

class MqttManagerIn
{
public:

  /**
   * @brief Construct a new Mqtt Manager object
   * 
   * The hostname used will be the chip ID
   * 
   * @param mqttServer Server name or IP Address
   * @param mqttPort TCP port number
   * @param *espConfig Pointer to ESPConfig object
   * @param *gpioManager Pointer ro GpioManager object
   */
  MqttManagerIn(String mqttServer, uint16_t mqttPort, ESPConfig *espConfig, GpioManager *gpioManager);
 
  /**
   * @brief Construct a new Mqtt Manager object
   * 
   * The hostname used will be the chip ID
   * 
   * @param mqttServer Server name or IP Address
   * @param mqttPort TCP port number
   * @param hostName (prefix "-in" will be added to hostname)
   * @param *espConfig Pointer to ESPConfig object
   * @param *gpioManager Pointer ro GpioManager object
   */
  MqttManagerIn(String mqttServer, uint16_t mqttPort, String hostName, ESPConfig *espConfig, GpioManager *gpioManager);

  /**
   * @brief Destroy the Mqtt Manager In object
   * 
   */
  ~MqttManagerIn();

  /**
   * @brief Connect to server
   * 
   * @return true Success
   * @return false Fail
   */
  bool connect();

  /**
   * @brief Check if connected
   * 
   * @return true Connected
   * @return false Disconnected
   */
  bool status();

  /**
   * @brief Handle MQTT for new messages
   * 
   */
  void handleMqtt();

private:

  /**
   * @brief Called when a new message arrives
   * 
   * @param topic Topic
   * @param payload Payload
   * @param length Message Lenght
   */
  static void callback(char *topic, byte *payload, unsigned int length, void *argLocal);

  void processSetGpio(ESPConfig *espConfig, GpioManager *gpioManager, JsonArray& statusGpio);

  WiFiClient _wifiClient;
  PubSubClient *_client;
  String _mqttServer;
  uint16_t _mqttPort;
  String _hostName;
  unsigned long _connectTimeTry = 0;
  ESPConfig *_espConfig;
  GpioManager *_gpioManager;
  struct configLocalStruct
  {
    ESPConfig *_espConfig;
    GpioManager *_gpioManager;
    MqttManagerIn *mqttManagerLocal;
  };

};

#endif