/**
 * @file ESPConfig.cpp
 * @author Fabricio Tamusiunas (fabricio@tamusiunas.com)
 * @brief Object to carry config parameters
 * @version 0.1
 * @date 2019-02-11
 * 
 * @copyright Copyright (c) 2019
 * 
 */

#include "ESPConfig.h"

ESPConfig::ESPConfig(int *pinGpioArray, int *pinGpioAvaliable, int *pinGpioAdcChannelArray, int *pinGpioAdcNumberArray,
          int *pinGpioInOut, const char **pinGpioDesc, int *pinPwmValue, int totalGpio, volatile int *pwmChannelGpioHw,
          volatile int pwmChannelTotalHw, volatile int *pwmChannelGpioSw, int pwmChannelTotalSw, int *pinGpioMode, volatile int *pinGpioDigitalStatusChanged, 
          volatile int *pinGpioDigitalStatus, volatile int *pinGpioAdcValue, volatile int *pinGpioAdcPreviousValue, 
          volatile bool *pinPwmEnable, volatile int *pinGpioPwmStatusChanged, volatile int *pinGpioPwmStatus, int *pinAnalogOnly, 
          volatile unsigned long *gpioDigitalActionIndexWhenReverseInMillis, DataStore *dataStore)

{
  _pwmChannelGpioHw = pwmChannelGpioHw;
  _pwmChannelTotalHw = pwmChannelTotalHw;
  _pwmChannelGpioSw = pwmChannelGpioSw;
  _pwmChannelTotalSw = pwmChannelTotalSw;
  _pinGpioArray = pinGpioArray;
  _pinGpioAvaliable = pinGpioAvaliable;
  _pinGpioAdcChannelArray = pinGpioAdcChannelArray;
  _pinGpioAdcNumberArray = pinGpioAdcNumberArray;
  _pinGpioInOut = pinGpioInOut;
  _pinGpioDesc = pinGpioDesc;
  _pinPwmValue = pinPwmValue;
  _pinGpioMode = pinGpioMode;
  _totalGpio = totalGpio;
  _pinGpioDigitalStatus = pinGpioDigitalStatus;
  _pinGpioDigitalStatusChanged = pinGpioDigitalStatusChanged;
  _pinGpioAdcValue = pinGpioAdcValue;
  _pinGpioAdcPreviousValue = pinGpioAdcPreviousValue;
  _pinPwmEnable = pinPwmEnable;
  _pinGpioPwmStatusChanged = pinGpioPwmStatusChanged; 
  _pinGpioPwmStatus = _pinGpioPwmStatus;
  _pinAnalogOnly = pinAnalogOnly;
  _gpioDigitalActionIndexWhenReverseInMillis = gpioDigitalActionIndexWhenReverseInMillis;
  _dataStore = dataStore;
}

volatile int *ESPConfig::getPinGpioPwmStatusChanged()
{
  return _pinGpioPwmStatusChanged;
}
  
volatile int *ESPConfig::getPinGpioPwmStatus()
{
  return _pinGpioPwmStatus;
}

int ESPConfig::addGpioToPwmChanneHw(int gpio)
{
  for (int cont=0 ; cont < _pwmChannelTotalHw ; cont++)
  {
    if (_pwmChannelGpioHw[cont] == -1)
    {
      _pwmChannelGpioHw[cont] = gpio;
      return cont;
    }
  }
  return -1;
}

volatile int ESPConfig::getPwmChannelTotalHw()
{
  return _pwmChannelTotalHw;
}

int ESPConfig::addGpioToPwmChanneSw(int gpio)
{
  for (int cont=0 ; cont < _pwmChannelTotalSw ; cont++)
  {
    if (_pwmChannelGpioSw[cont] == -1)
    {
      _pwmChannelGpioSw[cont] = gpio;
      return cont;
    }
  }
  return -1;
}

volatile int *ESPConfig::getPinGpioAdcValue()
{
  return _pinGpioAdcValue;
}

volatile int *ESPConfig::getPinGpioAdcPreviousValue()
{
  return _pinGpioAdcPreviousValue;
}

volatile bool *ESPConfig::getPinPwmEnable()
{
  return _pinPwmEnable;
}

volatile int *ESPConfig::getPwmChannelHw()
{
  return _pwmChannelGpioHw;
}

volatile int ESPConfig::getPwmChannelHwByGpio(int gpio)
{
  for (int cont=0 ; cont < _pwmChannelTotalHw ; cont++)
  {
    if (_pwmChannelGpioHw[cont] == gpio)
    {
      return cont;
    }
  }
  return -1;
}

int ESPConfig::getPwmChannelSwByGpio(int gpio)
{
  for (int cont=0 ; cont < _pwmChannelTotalSw ; cont++)
  {
    if (_pwmChannelGpioSw[cont] == gpio)
    {
      return cont;
    }
  }
  return -1;
}

int *ESPConfig::getPinGpioMode()
{
  return _pinGpioMode;
}

volatile int *ESPConfig::getPinGpioDigitalStatus()
{
  return _pinGpioDigitalStatus;
}

volatile int *ESPConfig::getPinGpioDigitalStatusChanged()
{
  return _pinGpioDigitalStatusChanged;
}

int *ESPConfig::getPinGpioArray()
{
  return _pinGpioArray;
}

void ESPConfig::setPinGpioAdcValue(int gpio, int AdcValue)
{
  _pinGpioAdcValue[gpio] = AdcValue;
}

void ESPConfig::setPinGpioAdcPreviousValue(int gpio, int AdcValue)
{
  _pinGpioAdcPreviousValue[gpio] = AdcValue;
}

void ESPConfig::setPinPwmEnable(int gpio, int isPwm)
{
  _pinPwmEnable[gpio] = isPwm;
}

void ESPConfig::setPinGpioDigitalStatus(int gpio, byte status)
{
  _pinGpioDigitalStatus[gpio] = status;
}

void ESPConfig::setPinGpioDigitalStatusChanged(int gpio, byte changed)
{
  _pinGpioDigitalStatusChanged[gpio] = changed;
}

void ESPConfig::setPinGpioMode(int gpio, byte mode)
{
  _pinGpioMode[gpio] = mode;
} 

void ESPConfig::setPinGpioPwmStatusChanged(int gpio, int changed)
{
  _pinGpioPwmStatusChanged[gpio] = changed;
}

void ESPConfig::setPinGpioPwmStatus(int gpio, int pwmStatus)
{
  _pinGpioPwmStatus[gpio] = pwmStatus;
}

int *ESPConfig::getPinAnalogOnly()
{
  return _pinAnalogOnly;
}

int *ESPConfig::getPinGpioAvaliable()
{
  return _pinGpioAvaliable;
}

int *ESPConfig::getPinGpioAdcChannelArray()
{
  return _pinGpioAdcChannelArray;
}

int *ESPConfig::getPinGpioAdcNumberArray()
{
  return _pinGpioAdcNumberArray;
}

int *ESPConfig::getPinGpioInOut()
{
  return _pinGpioInOut;
}

const char **ESPConfig::getPinGpioDesc()
{
  return _pinGpioDesc;
}

int *ESPConfig::getPinPwmValue()
{
  return _pinPwmValue;
}

int ESPConfig::getTotalGpio()
{
  return _totalGpio;
}

DataStore *ESPConfig::getDataStore()
{
  return _dataStore;
}

volatile unsigned long *ESPConfig::getGpioDigitalActionIndexWhenReverseInMillis()
{
  return _gpioDigitalActionIndexWhenReverseInMillis;
}
