/**
 * @file JsonManager.h
 * @author Fabricio Tamusiunas (fabricio@tamusiunas.com)
 * @brief Format JSON Messages
 * @version 0.1
 * @date 2019-02-09
 * 
 * @copyright Copyright (c) 2019
 * 
 */

#ifndef JsonManager_h
#define JsonManager_h

#include <ArduinoJson.h>

class JsonManager
{
public:
  /**
   * @brief Construct a new Json Manager object
   * 
   */
  JsonManager();
  /**
   * @brief Destroy the Json Manager object
   * 
   */
  ~JsonManager();
  /**
  * @brief Format JSON Message with header
  * 
  * @param mqttKey Array with MQTT index
  * @param mqttValue Array with MQTT values
  * @param mqttLenght MQTT array lenght
  * @param jsonHeader Json header message
  * @return String Json Formated
  */
  String formatJson(String mqttKey[], String mqttValue[], int mqttLenght, String jsonHeader);
  
  /**
  * @brief Format JSON Message without header
  * 
  * @param mqttKey Array with MQTT index
  * @param mqttValue Array with MQTT values
  * @param mqttLenght MQTT array lenght
  * @return String Json Formated
  */
  String formatJson(String mqttKey[], String mqttValue[], int mqttLenght);
};

#endif
