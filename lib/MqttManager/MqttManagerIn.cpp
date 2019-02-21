#include "MqttManagerIn.h"

MqttManagerIn::MqttManagerIn(String mqttServer, uint16_t mqttPort, ESPConfig *espConfig, GpioManager *gpioManager)
{
    _mqttServer = mqttServer;
    _mqttPort = mqttPort;
    _espConfig = espConfig;
    _gpioManager = gpioManager;
    _hostName = "iot-" + String(WifiGetChipId()) + "-in";
}

MqttManagerIn::MqttManagerIn(String mqttServer, uint16_t mqttPort, String hostName, ESPConfig *espConfig, GpioManager *gpioManager)
{
    _mqttServer = mqttServer;
    _mqttPort = mqttPort;
    _espConfig = espConfig;
    _gpioManager = gpioManager;
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
  if (_client.connected())
  {
    _client.loop();
  }
}

bool MqttManagerIn::connect()
{
  _client.setServer(_mqttServer.c_str(), _mqttPort);
  if (_client.connect(_hostName.c_str()))
  {
    _client.subscribe(_hostName.c_str());
    _client.setCallback(callback);
  }
  return _client.connected();
}

void MqttManagerIn::callback(char *topic, byte *payload, unsigned int length)
{
  String mqttMsg = "";
  for(int cont = 0; cont < length; cont++) 
  {
    mqttMsg += (char) payload[cont];
  }
  Serial.println("Received an MQTT message.");
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
    }
  }

}

bool MqttManagerIn::status()
{
  return _client.connected();
}
