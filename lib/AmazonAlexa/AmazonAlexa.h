#ifndef AmazonAlexa_h
#define AmazonAlexa_h

#include <fauxmoESP.h>
#include "AmazonAlexaCommons.h"
#include "DebugMessage.h"

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