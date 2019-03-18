#ifndef AmazonAlexa_h
#define AmazonAlexa_h

#include <fauxmoESP.h>
#include "AmazonAlexaCommons.h"
#include "DebugMessage.h"

class AmazonAlexa
{
public:
    AmazonAlexa(AlexaStruct *alexaStruct, volatile PwmAdcData *pwmAdcData, unsigned long tcpPort, DebugMessage *debugMessage);
    AmazonAlexa(AlexaStruct *alexaStruct, volatile PwmAdcData *pwmAdcData, DebugMessage *debugMessage);
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
    DebugMessage *_debugMessage;
};

#endif