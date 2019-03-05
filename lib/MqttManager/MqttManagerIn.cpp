#include "MqttManagerIn.h"

MqttManagerIn::MqttManagerIn(String mqttServer, uint16_t mqttPort, ESPConfig *espConfig, GpioManager *gpioManager, volatile PwmAdcData *pwmAdcDataLocal)
{
    _espConfig = espConfig;
    _gpioManager = gpioManager;

    configLocalStruct *configLocalStr = (configLocalStruct *)malloc(sizeof(configLocalStruct));
    configLocalStr->_espConfig = espConfig;
    configLocalStr->_gpioManager = gpioManager;
    configLocalStr->_pwmAdcDataLocal = pwmAdcDataLocal;
    configLocalStr->mqttManagerLocal = this;
    _client = new PubSubClient(_wifiClient, configLocalStr);
    _mqttServer = mqttServer;
    _mqttPort = mqttPort;
    _hostName = "iot-" + String(WifiGetChipId()) + "-in";
}

MqttManagerIn::MqttManagerIn(String mqttServer, uint16_t mqttPort, String hostName, ESPConfig *espConfig, GpioManager *gpioManager, volatile PwmAdcData *pwmAdcDataLocal)
{
    _espConfig = espConfig;
    _gpioManager = gpioManager;
    
    configLocalStruct *configLocalStr = (configLocalStruct *)malloc(sizeof(configLocalStruct));
    configLocalStr->_espConfig = espConfig;
    configLocalStr->_gpioManager = gpioManager;
    configLocalStr->_pwmAdcDataLocal = pwmAdcDataLocal;
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

  JsonArray& statusGpio = root["SetDigitalGpio"];
  if (statusGpio.size() > 0)
  {
    configCallbackStr->mqttManagerLocal->processSetDigitalGpio(configCallbackStr->_espConfig, configCallbackStr->_gpioManager, statusGpio);
  }
  else
  {
    JsonArray& statusGpio = root["SetPwmGpio"];
    if (statusGpio.size() > 0)
    {
      configCallbackStr->mqttManagerLocal->processSetPwmGpio(configCallbackStr->_espConfig, configCallbackStr->_gpioManager, configCallbackStr->_pwmAdcDataLocal, statusGpio);
      Serial.println("It's a SetPwmGpio");
    }
  }
}

bool MqttManagerIn::status()
{
  return _client->connected();
}

void MqttManagerIn::processSetPwmGpio(ESPConfig *espConfig, GpioManager *gpioManager, volatile PwmAdcData *pwmAdcDataLocal, JsonArray& statusGpio)
{
  for (JsonObject& elem : statusGpio)
  {
    int gpioInt = elem["gpio"].as<int>();
    String valueStr = elem["value"].as<String>();
    Serial.println("GPIO: " + String(gpioInt));
    Serial.println("Value: " + valueStr);
    if ((gpioInt >= 0) and (gpioInt < espConfig->getTotalGpio()))
    {
      if (_espConfig->getPinPwmEnable()[gpioInt] == true)
      {
        Serial.println("GPIO " + String(gpioInt) + " is configured as Pwm");
        int pwmValue = valueStr.toInt();
        gpioManager->setPwm(gpioInt,pwmValue, pwmAdcDataLocal);
      }
      else
      {
        Serial.println("GPIO " + String(gpioInt) + " is not configured as pwm");
      }
      
    }
  }
}

void MqttManagerIn::processSetDigitalGpio(ESPConfig *espConfig, GpioManager *gpioManager, JsonArray& statusGpio)
{
  for (JsonObject& elem : statusGpio)
  {
    int gpioInt = elem["gpio"].as<int>();
    String valueStr = elem["value"].as<String>();
    Serial.println("GPIO: " + String(gpioInt));
    Serial.println("Value: " + valueStr);
    if ((gpioInt >= 0) and (gpioInt < espConfig->getTotalGpio()))
    {
      if (_espConfig->getPinPwmEnable()[gpioInt] == false)
      {
        if (espConfig->getPinGpioMode()[gpioInt] == OUTPUT)
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
            gpioManager->setDigitalOutput(gpioInt,gpioMode);
            espConfig->setPinGpioDigitalStatusChanged(gpioInt,1);
            espConfig->setPinGpioDigitalStatus(gpioInt,gpioMode);
          }
        }
        else
        {
          Serial.println("GPIO " + String(gpioInt) + " is not configured as output");
        } 
      }
      else
      {
        Serial.println("GPIO " + String(gpioInt) + " is configured as pwm");
      }
    }
  }
}
