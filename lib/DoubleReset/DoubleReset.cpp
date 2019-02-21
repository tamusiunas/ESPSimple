/**
 * @file DoubleReset.cpp
 * @author Fabricio Tamusiunas (fabricio@tamusiunas.com)
 * @brief Check if reset button was pressed twice to enter in web config mode
 * @version 0.1
 * @date 2019-02-11
 * 
 * @copyright Copyright (c) 2019
 * 
 */

#include "DoubleReset.h"

DoubleReset::DoubleReset(unsigned int timeout)
{
  _timeout = timeout;
}

DoubleReset::DoubleReset()
{
  _timeout = 5000;
}

DoubleReset::~DoubleReset()
{
  _resetValue = 0;
}

void DoubleReset::setTimeout(unsigned int timeout)
{
  _timeout = timeout;
}

int DoubleReset::getResetValue()
{
  return _resetValue;
}

void DoubleReset::start()
{
  _started = true;
  _timeInMillis = millis();
  EEPROM.begin(sizeof(_resetValue));
  _resetValue = EEPROM.read(0);
  EEPROM.write(0, (_resetValue + 1));
  EEPROM.commit();
}

void DoubleReset::stop()
{
  EEPROM.write(0, 0);
  EEPROM.commit();
  _isTimeOut = true;
  _started = false;
}

void DoubleReset::handle()
{
  if ((!_isTimeOut == true) and (_started))
  {
    if ((millis() - _timeInMillis) > _timeout)
    {
      stop();
    }
  }
}
