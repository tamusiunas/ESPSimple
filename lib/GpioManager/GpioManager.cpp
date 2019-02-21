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

GpioManager::GpioManager(ESPConfig *espConfig, MqttManagerOut *mqttManagerOut)
{
  _espConfig = espConfig;
  _mqttManagerOut = mqttManagerOut;
  initializeTimers();
}

GpioManager::GpioManager(ESPConfig *espConfig)
{
  _espConfig = espConfig;
  _mqttManagerOut = NULL;
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
      }
    }
  }
}

void GpioManager::setGpioMode(int gpio, byte mode)
{
  pinMode(gpio, mode);
  Serial.println("setGpioMode: " + String(gpio) + "(" + String(mode) + ")");
}

ESPConfig *GpioManager::getESPConfig()
{
  return _espConfig;
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

bool GpioManager::setDigitalOutput(uint32_t gpio, uint32_t status)
{
  digitalWrite(gpio,status);
}