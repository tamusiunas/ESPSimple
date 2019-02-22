#include "MqttManagerOut.h"

MqttManagerOut::MqttManagerOut(String mqttServer, uint16_t mqttPort)
{
  _mqttServer = mqttServer;
  _mqttPort = mqttPort;
  _hostName = "iot-" + String(WifiGetChipId()) + "-out";
}

MqttManagerOut::MqttManagerOut(String mqttServer, uint16_t mqttPort, String hostName)
{
  _mqttServer = mqttServer;
  _mqttPort = mqttPort;
  _hostName = hostName + "-out";
}

MqttManagerOut::~MqttManagerOut()
{

}

bool MqttManagerOut::connect()
{
  _client.setServer(_mqttServer.c_str(), _mqttPort);
  if (_client.connect(_hostName.c_str()))
  {

  }
  return _client.connected();
}

bool MqttManagerOut::disconnect()
{
  _client.disconnect();
  return !_client.connected();
}

bool MqttManagerOut::status()
{
  return _client.connected();
}

bool MqttManagerOut::publishMessage(String mqttMessage)
{
  // to do: use a queue
  return _client.publish(_hostName.c_str(), mqttMessage.c_str());
}

bool MqttManagerOut::publishMessageJson(String mqttKey[], String mqttValue[], int arraySize, String title)
{
  JsonManager *jsonManager = new JsonManager();
  return publishMessage(jsonManager->formatJson(mqttKey,mqttValue,arraySize,title.c_str()));
}

void MqttManagerOut::handleMqtt()
{
  if ((_connectTimeTry + 10000) < millis())
  {
    if (MqttManagerOut::status() == false)
    {
      MqttManagerOut::connect();
      _connectTimeTry = millis();
    }
  }
  if (_client.connected())
  {
    _client.loop();
  }
}