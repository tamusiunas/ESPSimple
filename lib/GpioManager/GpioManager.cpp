/**
 * @file GpioManager.cpp
 * @author Fabricio Tamusiunas (fabricio@tamusiunas.com)
 * @brief Read and configure GPIO (mode and PWM)
 * @version 0.1
 * @date 2019-02-09
 * 
 * @copyright Copyright (c) 2019
 * 
 */
#include "GpioManager.h"

/*GpioManager::GpioManager(ESPConfig *espConfig, MqttManagerOut *mqttManagerOut)
{
  _espConfig = espConfig;
  _mqttManagerOut = mqttManagerOut;
  initializeTimers();
}*/

GpioManager::GpioManager(ESPConfig *espConfig)
{
  _espConfig = espConfig;
  //_mqttManagerOut = NULL;
  initializeTimers();
}

GpioManager::~GpioManager()
{

}

void GpioManager::initializeGpio()
{
  int totalParameters = _espConfig->getDataStore()->getDataParametersCount();
  Serial.println("totalParameters: " + String (totalParameters));
  for (int cont=0 ; cont < totalParameters ; ++cont )
  {
    DataParameter *dataParameter = _espConfig->getDataStore()->getParameterByPos(cont);
    if (dataParameter != NULL)
    {
      if (String(dataParameter->getField()).indexOf("gpio_mode_") == 0)
      {
        int gpioInt = String(String(dataParameter->getField()).substring(10)).toInt();
        const char *gpioModeCChar = dataParameter->getValue();
        Serial.println("Found Gpio Mode: " + String(gpioInt) + "(" + String(gpioModeCChar) + ")");
        if (strcmp(gpioModeCChar,"input_pullup") == 0)
        {
          setGpioMode(gpioInt,INPUT_PULLUP);
          setInterrupt(gpioInt);
        }
        else
        if (strcmp(gpioModeCChar,"input") == 0)
        {
          setGpioMode(gpioInt,INPUT);
        }
        else
        if (strcmp(gpioModeCChar,"output") == 0)
        {
          setGpioMode(gpioInt,OUTPUT);
        }
      }
      else
      if (String(dataParameter->getField()).indexOf("pwm_gpio_r_") == 0)
      {
        int pwmPosition = String(String(dataParameter->getField()).substring(11)).toInt();
        int PwmGpio = String(dataParameter->getValue()).toInt();
        String gpioSource = String(_espConfig->getDataStore()->getValue(String("pwm_source_r_" + String(pwmPosition)).c_str()));
        Serial.println("Found PWM: " + String(PwmGpio) + "-" + gpioSource);
        if (gpioSource == "zerocross")
        {
          addGpioPwmZeroCross(PwmGpio);
        }
        else
        if (gpioSource == "nonzerocross")
        {
          addGpioPwmNonZeroCross(PwmGpio);
        }
        _espConfig->setPinPwmEnable(PwmGpio,true);
      }
    }
  }
}

void GpioManager::setGpioMode(int gpio, byte mode)
{
  pinMode(gpio, mode);
  _espConfig->setPinGpioMode(gpio,mode);
  Serial.println("setGpioMode: " + String(gpio) + "(" + String(mode) + ")");
}

ESPConfig *GpioManager::getESPConfig()
{
  return _espConfig;
}

void GpioManager::checkGpioChange(MqttManagerOut *mqttManagerOut, volatile PwmAdcData *pwmAdcDataLocal)
{
  // for (int cont = 0; cont < _espConfig->getTotalGpio(); cont++)
  for (int cont = 0; cont < pwmAdcDataLocal->totalGPIO; cont++)
  {
    //if (_espConfig->getPinGpioDigitalStatusChanged()[cont] == 1)
    if (pwmAdcDataLocal->pinGpioDigitalStatusChanged[cont] == 1)
    {
      //int pinGpioDigitalStatusLocal = _espConfig->getPinGpioDigitalStatus()[cont];
      int pinGpioDigitalStatusLocal = pwmAdcDataLocal->pinGpioDigitalStatus[cont];
      Serial.println("Gpio status changed: " + String(cont) + ". New status: " + String(pinGpioDigitalStatusLocal));
      String mqttKey[2];
      String mqttValue[2];
      mqttKey[0] = "gpio";
      mqttValue[0] = String(cont);
      mqttKey[1] = "status";
      if (pinGpioDigitalStatusLocal == HIGH)
      {
        mqttValue[1] = "high";
      }
      else
      {
        mqttValue[1] = "low";
      }
      //_espConfig->getPinGpioDigitalStatusChanged()[cont] = 0;
      pwmAdcDataLocal->pinGpioDigitalStatusChanged[cont] = 0;
      //Serial.println("Sending " + mqttValue[0]);
      mqttManagerOut->publishMessageJson(mqttKey, mqttValue, 2, "InfoDigitalGpio");
    }
    //if (_espConfig->getPinGpioPwmStatusChanged()[cont] == 1)
    if (pwmAdcDataLocal->pinGpioPwmStatusChanged[cont] == 1)
    {
      // int pinGpioPwmStatusLocal = _espConfig->getPinGpioPwmStatus()[cont];
      int pinGpioPwmStatusLocal = pwmAdcDataLocal->pinGpioPwmStatus[cont];
      Serial.println("Gpio Pwm status changed: " + String(cont) + ". New status: " + String(pinGpioPwmStatusLocal));
      String mqttKey[2];
      String mqttValue[2];
      mqttKey[0] = "gpio";
      mqttValue[0] = String(cont);
      mqttKey[1] = "status";
      mqttValue[1] = String(pinGpioPwmStatusLocal);
      // _espConfig->getPinGpioPwmStatusChanged()[cont] = 0;
      pwmAdcDataLocal->pinGpioPwmStatusChanged[cont] = 0;
      //Serial.println("Sending " + mqttValue[0]);
      mqttManagerOut->publishMessageJson(mqttKey, mqttValue, 2, "InfoPwmGpio");
    }
  }
}

void GpioManager::executeDigitalAction(uint32_t gpio, int gpioStatus)
{
  int digitalActionTotalInt = String(_espConfig->getDataStore()->getValue("action_digital_total")).toInt();
  Serial.print("Total of digital actions: ");
  Serial.println(digitalActionTotalInt);
}

uint32_t GpioManager::getDigitalOutput(uint32_t gpio)
{
  return (digitalRead(gpio));
}

void GpioManager::setDigitalOutput(uint32_t gpio, uint32_t status)
{
  digitalWrite(gpio,status);
  String mqttKey[1];
  String mqttValue[1];
  mqttKey[0] = "gpio";
  if (status == HIGH)
  {
    mqttValue[0] = "high";
  }
  else
  {
    mqttValue[0] = "low";
  }
}