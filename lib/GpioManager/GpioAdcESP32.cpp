#ifdef ESP32

#include "GpioManager.h"

int GpioManager::getAdcValue(String gpioAdc)
{
    int gpioAdcInt = gpioAdc.toInt();
    int adc = -1;
    int adcChannel = -1;
    if (_espConfig->getPinGpioAdcChannelArray()[gpioAdcInt] == 1)
    {
        adc = 1;
    }
    else if (_espConfig->getPinGpioAdcChannelArray()[gpioAdcInt] == 2)
    {
        adc = 2;
    }
    if (adc != -1)
    {
        switch (gpioAdcInt)
        {
            case 36:
                adcChannel = 0;
                break;
            case 37:
                adcChannel = 1;
                break;
            case 38:
                adcChannel = 2;
                break;
            case 39:
                adcChannel = 3;
                break;
            case 32:
                adcChannel = 4;
                break;
            case 33:
                adcChannel = 5;
                break;
            case 34:
                adcChannel = 6;
                break;
            case 35:
                adcChannel = 7;
                break;
            case 4:
                adcChannel = 0;
                break;
            case 0:
                adcChannel = 1;
                break;
            case 2:
                adcChannel = 2;
                break;
            case 15:
                adcChannel = 3;
                break;
            case 13:
                adcChannel = 4;
                break;
            case 12:
                adcChannel = 5;
                break;
            case 14:
                adcChannel = 6;
                break;
            case 27:
                adcChannel = 7;
                break;
            case 25:
                adcChannel = 8;
                break;
            case 26:
                adcChannel = 9;
                break;
        }
        if (adc == 1)
        {
            return adc1_get_raw((adc1_channel_t) adcChannel);
        }
        else if (adc == 2)
        {
            // Adc can be used without WiFi, so it's not really useful for now
            return -1;
        }
    }
    else
    return -1;
    
}

void GpioManager::configAdc(String gpioAdc)
{
    int gpioAdcInt = gpioAdc.toInt();
    int adc = -1;
    int adcChannel = -1;
    int adcAtten = 0;
    if (_espConfig->getPinGpioAdcChannelArray()[gpioAdcInt] == 1)
    {
        adc = 1;
    }
    else if (_espConfig->getPinGpioAdcChannelArray()[gpioAdcInt] == 2)
    {
        adc = 2;
    }
    if (adc != -1)
    {
        switch (gpioAdcInt)
        {
            case 36:
                adcChannel = 0;
                break;
            case 37:
                adcChannel = 1;
                break;
            case 38:
                adcChannel = 2;
                break;
            case 39:
                adcChannel = 3;
                break;
            case 32:
                adcChannel = 4;
                break;
            case 33:
                adcChannel = 5;
                break;
            case 34:
                adcChannel = 6;
                break;
            case 35:
                adcChannel = 7;
                break;
            case 4:
                adcChannel = 0;
                break;
            case 0:
                adcChannel = 1;
                break;
            case 2:
                adcChannel = 2;
                break;
            case 15:
                adcChannel = 3;
                break;
            case 13:
                adcChannel = 4;
                break;
            case 12:
                adcChannel = 5;
                break;
            case 14:
                adcChannel = 6;
                break;
            case 27:
                adcChannel = 7;
                break;
            case 25:
                adcChannel = 8;
                break;
            case 26:
                adcChannel = 9;
                break;
        }

        String attenuationStr = "gpio_adc_atten_" + gpioAdc;
        String adcAttenStr = _espConfig->getDataStore()->getValue(attenuationStr.c_str());

        if (adcAttenStr == "2.5")
        {
            adcAtten = 1; // attenuation 2.5 DB. The input voltage of ADC will be reduced to about 1/1.34

        }
        else if (adcAttenStr == "6")
        {
            adcAtten = 2; // attenuation 6 DB. The input voltage of ADC will be reduced to about 1/2

        }
        else if (adcAttenStr == "11")
        {
            adcAtten = 3; // attenuation 11 DB. The input voltage of ADC will be reduced to about 1/3.6

        }
        
        if (adc == 1)
        {
            _debugMessage->debug("Configuring ADC1: Channel: " + String(adcChannel) + " - Attenuation: " + String(adcAttenStr) + " db");
            adc1_config_width(ADC_WIDTH_BIT_10);
            adc1_config_channel_atten((adc1_channel_t) adcChannel, (adc_atten_t) adcAtten);
        }
        else if (adc == 2)
        {
            // Adc can be used without WiFi, so it's not really useful for now
        }
    }
}

#endif