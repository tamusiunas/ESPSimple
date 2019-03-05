#ifndef AmazonAlexa_h
#define AmazonAlexa_h

#include <fauxmoESP.h>
#include "ESPConfig.h"

class AmazonAlexa
{
public:
    AmazonAlexa(ESPConfig *espConfig, unsigned long tcpPort);
    ~AmazonAlexa();
    void Handle();
    void AddDevice(const char *deviceName);
private:
    fauxmoESP *_fauxmo;
    ESPConfig *_espConfig;
};

#endif