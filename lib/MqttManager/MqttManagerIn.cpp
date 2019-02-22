#include "MqttManagerIn.h"

MqttManagerIn::MqttManagerIn(String mqttServer, uint16_t mqttPort, ESPConfig *espConfig, GpioManager *gpioManager)
{
    _espConfig = espConfig;
    _gpioManager = gpioManager;

    configLocalStruct *configLocalStr = (configLocalStruct *)malloc(sizeof(configLocalStruct));
    configLocalStr->_espConfig = espConfig;
    configLocalStr->_gpioManager = gpioManager;
    _client = new PubSubClient(_wifiClient, configLocalStr);
    Serial.println("Testing -> " + configLocalStr->_espConfig->getTotalGpio());

    _mqttServer = mqttServer;
    _mqttPort = mqttPort;
    _hostName = "iot-" + String(WifiGetChipId()) + "-in";
}

MqttManagerIn::MqttManagerIn(String mqttServer, uint16_t mqttPort, String hostName, ESPConfig *espConfig, GpioManager *gpioManager)
{
    _espConfig = espConfig;
    _gpioManager = gpioManager;
    
    configLocalStruct *configLocalStr = (configLocalStruct *)malloc(sizeof(configLocalStruct));
    configLocalStr->_espConfig = espConfig;
    configLocalStr->_gpioManager = gpioManager;
    _client = new PubSubClient(_wifiClient, configLocalStr);

    _mqttServer = mqttServer;
    _mqttPort = mqttPort;
    _hostName = hostName + "-in";
}

MqttManagerIn::~MqttManagerIn()
{

}

void MqttManagerIn::handleMqtt()
{
  if ((_connectTimeTry + 10000) < millis())
  {
    if (MqttManagerIn::status() == false)
    {
      MqttManagerIn::connect();
      _connectTimeTry = millis();
    }
  }
  if (_client->connected())
  {
    _client->loop();
  }
}

bool MqttManagerIn::connect()
{
  _client->setServer(_mqttServer.c_str(), _mqttPort);
  if (_client->connect(_hostName.c_str()))
  {
    _client->subscribe(_hostName.c_str());
    _client->setCallback(callback);
  }
  return _client->connected();
}

void MqttManagerIn::callback(char *topic, byte *payload, unsigned int length, void *argLocal)
{
  volatile configLocalStruct *configCallbackStr = (configLocalStruct*) argLocal;
  String mqttMsg = "";
  for(int cont = 0; cont < (int) length; cont++) 
  {
    mqttMsg += (char) payload[cont];
  }
  Serial.println("Received an MQTT message.");
  Serial.println("Testing -> " + String(configCallbackStr->_espConfig->getTotalGpio()));
  Serial.println("Topic: " + String(topic));
  Serial.println("Message: " + mqttMsg);

  DynamicJsonBuffer jsonBuffer;
  JsonObject& root = jsonBuffer.parseObject(mqttMsg);
  JsonArray& statusGpio = root["SetGpio"];
  if (statusGpio.size() > 0)
  {
    for (JsonObject& elem : statusGpio)
    {
      int gpioInt = elem["gpio"].as<int>();
      String valueStr = elem["value"].as<String>();
      Serial.println("GPIO: " + String(gpioInt));
      Serial.println("Value: " + valueStr);
      if ((gpioInt >= 0) and (gpioInt < configCallbackStr->_espConfig->getTotalGpio()))
      {
        if (configCallbackStr->_espConfig->getPinGpioMode()[gpioInt] == OUTPUT)
        {
          Serial.println("GPIO " + String(gpioInt) + " is configured as output");
          uint32_t gpioMode = -1;
          if (valueStr == "high")
          {
            gpioMode = HIGH;
          }
          else if (valueStr == "low")
          {
            gpioMode = LOW;
          }
          if (gpioMode >= 0)
          {
            configCallbackStr->_gpioManager->setDigitalOutput(gpioInt,gpioMode);
            configCallbackStr->_espConfig->setPinGpioStatusChanged(gpioInt,1);
            configCallbackStr->_espConfig->setPinGpioDigitalStatus(gpioInt,gpioMode);
          }
        }
        else
        {
          Serial.println("GPIO " + String(gpioInt) + " is not configured as output");
        } 
      }
    }
  }

}

bool MqttManagerIn::status()
{
  return _client->connected();
}
