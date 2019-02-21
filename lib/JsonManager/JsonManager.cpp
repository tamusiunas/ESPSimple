/**
 * @file JsonManager.cpp
 * @author Fabricio Tamusiunas (fabricio@tamusiunas.com)
 * @brief Format JSON Messages
 * @version 0.1
 * @date 2019-02-09
 * 
 * @copyright Copyright (c) 2019
 * 
 */

#include <Arduino.h>
#include "JsonManager.h"

JsonManager::JsonManager()
{

}

String JsonManager::formatJson(String mqttKey[], String mqttValue[], int mqttLenght, String jsonHeader)
{
  const size_t bufferSize = JSON_OBJECT_SIZE(mqttLenght);
  DynamicJsonBuffer jsonBuffer(bufferSize);
  JsonObject& root = jsonBuffer.createObject();
  JsonObject& headerJson = root.createNestedObject(jsonHeader.c_str());
  for (int cont=0; cont<mqttLenght; cont++)
  {
    headerJson[mqttKey[cont]] = mqttValue[cont];
  }
  String jsonFormated;
  root.printTo(jsonFormated);
  return jsonFormated;
}

String JsonManager::formatJson(String mqttKey[], String mqttValue[], int mqttLenght)
{
  const size_t bufferSize = JSON_OBJECT_SIZE(mqttLenght);
  DynamicJsonBuffer jsonBuffer(bufferSize);
  JsonObject& root = jsonBuffer.createObject();
  for (int cont=0; cont<mqttLenght; cont++)
  {
    root[mqttKey[cont]] = mqttValue[cont];
  }
  String jsonFormated;
  root.printTo(jsonFormated);
  return jsonFormated;
}