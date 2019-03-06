#ifndef AmazonAlexa_h
#define AmazonAlexa_h

#include <fauxmoESP.h>
#include "AmazonAlexaCommons.h"

class AmazonAlexa
{
public:
    AmazonAlexa(AlexaStruct *alexaStruct, volatile PwmAdcData *pwmAdcData, unsigned long tcpPort);
    AmazonAlexa(AlexaStruct *alexaStruct, volatile PwmAdcData *pwmAdcData);
    ~AmazonAlexa();
    void Handle();
    void Enable();
    void Disable();
    void AddDevice(const char *deviceName);
private:
    fauxmoESP *_fauxmo;
    
    AlexaStruct *_alexaStruct;
    volatile PwmAdcData *_pwmAdcData;
};

#endif