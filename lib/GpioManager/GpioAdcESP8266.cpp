#ifdef ESP8266

#include "GpioManager.h"

int GpioManager::getAdcValue(String gpioAdc)
{
    int gpioPin = gpioAdc.substring(1).toInt();

    int adcValue = 0;
    for (int adcCont = 0 ; adcCont < 32 ; adcCont++)
    {
        adcValue += analogRead(gpioPin);
    }
    adcValue = adcValue/32;

    return adcValue;
}

void GpioManager::configAdc(String gpioAdc)
{
    // ESP8266 analog pin doesn't need configuration 
}

#endif