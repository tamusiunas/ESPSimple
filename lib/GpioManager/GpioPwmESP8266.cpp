/**
 * @file GpioPwmESP8266.cpp
 * @author Fabricio Tamusiunas (fabricio@tamusiunas.com)
 * @brief Read and configure GPIO (methods for ESP8266)
 * @version 0.1
 * @date 2019-02-09
 * 
 * @copyright Copyright (c) 2019
 * 
 */

#ifdef ESP8266

#include "GpioManager.h"

void GpioManager::configGpioPwm(uint32_t gpio)
{

}

void GpioManager::initializeTimers()
{
  int frequency = 60;
  int zeroCrossGpio = -1;
  if (String(_espConfig->getDataStore()->getValue("pwm_enable_zero_cross")) == "yes")
  {
    _zeroCrossEnable = true;
    String zeroCrossFrequencyStr = _espConfig->getDataStore()->getValue("pwm_zero_cross_frequency");
    if (zeroCrossFrequencyStr != "auto")
    {
      frequency = zeroCrossFrequencyStr.toInt();
    }
    else
    {
      // To-Do Configure Zero Cross Auto
    }
  }
  if (_zeroCrossEnable)
  {
    zeroCrossGpio = String(_espConfig->getDataStore()->getValue("pwm_zero_cross_gpio")).toInt();
    attachInterrupt(digitalPinToInterrupt(zeroCrossGpio), std::bind(&GpioManager::handleZeroCrossInterrupt,this), RISING);
    int totalSteps = PWMSTEPS;
    // To-do: set timer
  }
}

void GpioManager::addGpioPwmZeroCross(int gpio)
{

}

void GpioManager::addGpioPwmNonZeroCross(int gpio)
{

}

void GpioManager::setPwm(int gpio, int pwm)
{
  if (String(_espConfig->getDataStore()->getValue("pwm_enable_zero_cross")) == "yes")
  {
    // To-do: enable Zero Cross for ESP8266
  }
  else
  {
    Serial.println("");
    Serial.println("Gpio: " + String(gpio));
    Serial.println("Pwm: " + String(pwm));
    analogWrite(gpio,pwm);
  }
}

void ICACHE_RAM_ATTR GpioManager::handleTimer()
{

}

void ICACHE_RAM_ATTR GpioManager::handleZeroCrossInterrupt()
{

}

#endif
