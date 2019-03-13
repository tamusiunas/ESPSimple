#ifdef ESP8266

#include "GpioManager.h"

int GpioManager::getAdcValue(String gpioAdc)
{
    int gpioPin = gpioAdc.substring(1).toInt();
    return analogRead(gpioPin);
}

void GpioManager::configAdc(String gpioAdc)
{
    // ESP8266 analog pin doesn't need configuration 
}

#endif