#include "MqttManagerIn.h"

MqttManagerIn::MqttManagerIn(String mqttServer, uint16_t mqttPort, ESPConfig *espConfig, GpioManager *gpioManager, volatile PwmAdcData *pwmAdcDataLocal, DebugMessage *debugMessage)
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

MqttManagerIn::MqttManagerIn(String mqttServer, uint16_t mqttPort, String hostName, ESPConfig *espConfig, GpioManager *gpioManager, volatile PwmAdcData *pwmAdcDataLocal, DebugMessage *debugMessage)
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
  DebugMessage debugMessageLocal = DebugMessage();
  volatile configLocalStruct *configCallbackStr = (configLocalStruct*) argLocal;
  String mqttMsg = (char *)payload;
  debugMessageLocal.debug("Received an MQTT message - Topic: " + String(topic) + "Message: " + mqttMsg);

  DynamicJsonBuffer jsonBuffer;

  JsonObject& root = jsonBuffer.parseObject(mqttMsg);

 for (JsonPair& jp1 : root) {
    debugMessageLocal.debug("key: " + String(jp1.key));
    JsonVariant jv =  jp1.value;

    if (!strcmp(jp1.key,"SetDigitalGpio"))
    {
      configCallbackStr->mqttManagerLocal->processSetDigitalGpio(configCallbackStr->_espConfig, 
                                                                 configCallbackStr->_gpioManager, jv);
    }
    else if (!strcmp(jp1.key,"SetPwmGpio"))
    {
        configCallbackStr->mqttManagerLocal->processSetPwmGpio(configCallbackStr->_espConfig, configCallbackStr->_gpioManager, 
                                                               configCallbackStr->_pwmAdcDataLocal, jv);
    }
    else if (!strcmp(jp1.key,"GetAdcGpio"))
    {
        configCallbackStr->mqttManagerLocal->processGetAdcGpio(configCallbackStr->_pwmAdcDataLocal, jv);
    }
    else if (!strcmp(jp1.key,"GetDHT"))
    {
        configCallbackStr->mqttManagerLocal->processGetDHT(configCallbackStr->_pwmAdcDataLocal, jv);
    }
  } 
  
}

void MqttManagerIn::processGetDHT(volatile PwmAdcData *pwmAdcDataLocal, JsonVariant& jv)
{

  DebugMessage debugMessageLocal = DebugMessage();
  int dhtId = -1;
  String dhtKey = "";
  for (const auto& kv : jv.as<JsonObject>())
  {
    debugMessageLocal.debug("key: " + String(kv.key));
    debugMessageLocal.debug("value: " + String(kv.value.as<int>()));
    if (!strcmp(kv.key, "dhtId"))
    {
      dhtId = kv.value.as<int>();
    }
    if (!strcmp(kv.key, "key"))
    {
      dhtKey = kv.value.as<const char *>();
    }
  }
  
  if (dhtKey == "humidity")
  {
    pwmAdcDataLocal->sendDhtHumidity[dhtId] = true;
  }
  else if (dhtKey == "celsius")
  {
    pwmAdcDataLocal->sendDhtCelsius[dhtId] = true;
  }
  else if (dhtKey == "fahrenheit")
  {
    pwmAdcDataLocal->sendDhtFahrenheit[dhtId] = true;
  }
  
}

void MqttManagerIn::processGetAdcGpio(volatile PwmAdcData *pwmAdcDataLocal, JsonVariant& jv)
{
  DebugMessage debugMessageLocal = DebugMessage();
  String gpioIdString = "";
  for (const auto& kv : jv.as<JsonObject>())
  {
    debugMessageLocal.debug("key: " + String(kv.key));
    debugMessageLocal.debug("value: " + String(kv.value.as<int>()));
    if (!strcmp(kv.key, "gpio"))
    {
      gpioIdString = kv.value.as<const char *>();
    }
  }
  
  bool isAnalogOnly = false;
  int gpioInt = -1;
  // It's fot ESP8266 which there are analog GPIO (A0, A1, etc.)
  if (gpioIdString.indexOf("a") == 0)
  {
    gpioInt = gpioIdString.substring(1).toInt();
    isAnalogOnly = true;
  }
  else
  {
    gpioInt = gpioIdString.toInt();
  }
  if ((gpioInt >= 0) and (gpioInt < pwmAdcDataLocal->totalGPIO))
  {
    // Check if it's ESP8266
    if (isAnalogOnly)
    {
      //Exclusive for ESP8266 (for this project)
      pwmAdcDataLocal->sendAdcAnalogOnlyValue[gpioInt] = 1;
    }
    else
    {
      pwmAdcDataLocal->sendAdcGpioValue[gpioInt] = 1;
    }
  }
  else
  {
    // send message Out of Range
  }
    
}

bool MqttManagerIn::status()
{
  return _client->connected();
}

void MqttManagerIn::processSetPwmGpio(ESPConfig *espConfig, GpioManager *gpioManager, volatile PwmAdcData *pwmAdcDataLocal, JsonVariant& jv)
{
  DebugMessage debugMessageLocal = DebugMessage();
  int gpioNumber = -1;
  int gpioPwmValue = -1;
  for (const auto& kv : jv.as<JsonObject>())
  {
    debugMessageLocal.debug("key: " + String(kv.key));
    debugMessageLocal.debug("value: " + String(kv.value.as<int>()));
    if (!strcmp(kv.key, "gpio"))
    {
      gpioNumber = kv.value.as<int>();
    }
    else if (!strcmp(kv.key, "value"))
    {
      gpioPwmValue = kv.value.as<int>();
    }
  }
  if ((gpioNumber >= 0) and (gpioNumber < espConfig->getTotalGpio()))
  {
    if (_espConfig->getPinPwmEnable()[gpioNumber] == true)
    {
      gpioManager->setPwm(gpioNumber,gpioPwmValue, pwmAdcDataLocal);
    }
  }
}

void MqttManagerIn::processSetDigitalGpio(ESPConfig *espConfig, GpioManager *gpioManager, JsonVariant& jv)
{
  DebugMessage debugMessageLocal = DebugMessage();
  int gpioNumber = -1;
  String gpioStatusValue = "";
  for (const auto& kv : jv.as<JsonObject>())
  {
    debugMessageLocal.debug("key: " + String(kv.key));
    debugMessageLocal.debug("value: " + String(kv.value.as<char*>()));
    if (!strcmp(kv.key, "gpio"))
    {
      gpioNumber = kv.value.as<int>();
    }
    else if (!strcmp(kv.key, "value"))
    {
      gpioStatusValue = kv.value.as<char*>();
    }
  }

  if ((gpioNumber >= 0) and (gpioNumber < espConfig->getTotalGpio()))
  {
    if (_espConfig->getPinPwmEnable()[gpioNumber] == false)
    {
      if (espConfig->getPinGpioMode()[gpioNumber] == OUTPUT)
      {
        uint32_t gpioMode = -1;
        if (gpioStatusValue == "high")
        {
          gpioMode = HIGH;
        }
        else if (gpioStatusValue == "low")
        {
          gpioMode = LOW;
        }
        if (gpioMode >= 0)
        {
          gpioManager->setDigitalOutput(gpioNumber,gpioMode);
          espConfig->setPinGpioDigitalStatusChanged(gpioNumber,1);
          espConfig->setPinGpioDigitalStatus(gpioNumber,gpioMode);
        }
      }
      else
      {
        debugMessageLocal.debug("GPIO " + String(gpioNumber) + " is not configured as OUTPUT");
      }
    }
    else
    {
      debugMessageLocal.debug("GPIO " + String(gpioNumber) + " is configured as PWM");
    }
  }
}
