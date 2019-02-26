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
          volatile int pwmChannelTotalHw, volatile int *pwmChannelGpioSw, int pwmChannelTotalSw, int *pinGpioMode, volatile int *pinGpioStatusChanged, 
          volatile int *pinGpioDigitalStatus, volatile int *pinGpioAdcValue, volatile int *pinGpioAdcPreviousValue, 
          volatile bool *pinPwmEnable, DataStore *dataStore)

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
  _pinGpioStatusChanged = pinGpioStatusChanged;
  _pinGpioAdcValue = pinGpioAdcValue;
  _pinGpioAdcPreviousValue = pinGpioAdcPreviousValue;
  _pinPwmEnable = pinPwmEnable;
  _dataStore = dataStore;
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

volatile int *ESPConfig::getPinGpioStatusChanged()
{
  return _pinGpioStatusChanged;
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

void ESPConfig::setPinGpioStatusChanged(int gpio, byte changed)
{
  _pinGpioStatusChanged[gpio] = changed;
}

void ESPConfig::setPinGpioMode(int gpio, byte mode)
{
  _pinGpioMode[gpio] = mode;
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
