#include "CheckMqtt.h"

CheckMqtt::CheckMqtt(MqttManagerOut *mqttManagerOut, volatile PwmAdcData *pwmAdcDataLocal, DebugMessage *debugMessage)
{
    _mqttManagerOut = mqttManagerOut;
    _pwmAdcDataLocal = pwmAdcDataLocal;
    _debugMessage = debugMessage;
}

void CheckMqtt::checkDht(DhtManager **dhtManagerArray)
{
  for (int cont = 0 ; cont < _pwmAdcDataLocal->totalDht ; cont++)
  {
    String mqttKey[3];
    String mqttValue[3];
    mqttKey[0] = "dhtId";
    mqttKey[1] = "key";
    mqttKey[2] = "value";
    mqttValue[0] = String(cont);
    if (_pwmAdcDataLocal->sendDhtHumidity[cont])
    {
      mqttValue[1] = "humidity";
      mqttValue[2] = String(dhtManagerArray[cont]->getHumidity());
      _pwmAdcDataLocal->sendDhtHumidity[cont] = false;
      _mqttManagerOut->publishMessageJson(mqttKey, mqttValue, 3, "InfoDht");
      _debugMessage->debug("Sent " + mqttValue[1] + " for DHT ID " + String(cont));
    }
    if (_pwmAdcDataLocal->sendDhtCelsius[cont])
    {
      mqttValue[1] = "celsius";
      mqttValue[2] = String(dhtManagerArray[cont]->getTemperatureCelsius());
      _pwmAdcDataLocal->sendDhtCelsius[cont] = false;
      _mqttManagerOut->publishMessageJson(mqttKey, mqttValue, 3, "InfoDht");
      _debugMessage->debug("Sent " + mqttValue[1] + " for DHT ID " + String(cont));
    }
    if (_pwmAdcDataLocal->sendDhtFahrenheit[cont])
    {
      mqttValue[1] = "fahrenheit";
      mqttValue[2] = String(dhtManagerArray[cont]->getTemperatureFahrenheit());
      _pwmAdcDataLocal->sendDhtFahrenheit[cont] = false;
      _mqttManagerOut->publishMessageJson(mqttKey, mqttValue, 3, "InfoDht");
      _debugMessage->debug("Sent " + mqttValue[1] + " for DHT ID " + String(cont));
    }
  }
}