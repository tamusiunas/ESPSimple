/**
 * @file MqttManagerOut.h
 * @author Fabricio Tamusiunas (fabricio@tamusiunas.com)
 * @brief Manager for MQTT outgoing messages
 * @version 0.1
 * @date 2019-02-21
 * 
 * @copyright Copyright (c) 2019
 * 
 */
#ifndef MqttManagerOut_h
#define MqttManagerOut_h

#define MQTT_MAX_PACKET_SIZE 2048
#include <PubSubClient.h>
#include "WiFiAliases.h"
#include "JsonManager.h"

class MqttManagerOut
{
public:

  /**
   * @brief Construct a new Mqtt Manager object
   * 
   * The hostname used will be the chip ID
   * 
   * @param mqttServer Server name or IP Address
   * @param mqttPort TCP port number
   */
  MqttManagerOut(String mqttServer, uint16_t mqttPort);
 
  /**
   * @brief Construct a new Mqtt Manager object
   * 
   * The hostname used will be the chip ID
   * 
   * @param mqttServer Server name or IP Address
   * @param mqttPort TCP port number
   * @param hostName Hostname
   */
  MqttManagerOut(String mqttServer, uint16_t mqttPort, String hostName);

  /**
   * @brief Destroy the Mqtt Manager In object
   * 
   */
  ~MqttManagerOut();

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
   * @brief Publish the message
   * 
   * @param mqttMessage Message
   * @return true Sucess
   * @return false Fail
   */
  bool publishMessage(String mqttMessage);

  void handleMqtt();

private:

  WiFiClient _wifiClient;
  PubSubClient _client = PubSubClient(_wifiClient);
  String _mqttServer;
  uint16_t _mqttPort;
  String _hostName;
  unsigned long _connectTimeTry = 0;
};
#endif