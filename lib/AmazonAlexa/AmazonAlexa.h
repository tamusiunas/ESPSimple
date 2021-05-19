/**
 * @file AmazonAlexa.h
 * @author Fabricio Tamusiunas (fabricio@tamusiunas.com)
 * @brief Uses FauxmoESP to integrate project with Amazon Alexa
 * @version 0.1
 * @date 2019-02-11
 * 
 * @copyright Copyright (c) 2019
 * 
 */

#ifndef AmazonAlexa_h
#define AmazonAlexa_h

#include <fauxmoESP.h>
#include "AmazonAlexaCommons.h"
#include "DebugMessage.h"

/**
 * @brief Uses FauxmoESP to integrate project with Amazon Alexa
 * 
 */
class AmazonAlexa
{
public:
    AmazonAlexa(AlexaStruct *alexaStruct, volatile PwmAdcData *pwmAdcData, unsigned long tcpPort);
    AmazonAlexa(AlexaStruct *alexaStruct, volatile PwmAdcData *pwmAdcData);
    ~AmazonAlexa();
    void handle();
    void enable();
    void disable();
    void addDevice(const char *deviceName);
    void addConfiguredDevices();
private:
    fauxmoESP *_fauxmo;
    
    AlexaStruct *_alexaStruct;
    volatile PwmAdcData *_pwmAdcData;
    DebugMessage _debugMessage;
};

#endif