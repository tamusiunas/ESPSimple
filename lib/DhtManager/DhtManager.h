#ifndef DhtManager_h
#define DhtManager_h

#include <DHT.h>
#include "DebugMessage.h"


class DhtManager
{
public:
    DhtManager();
    DhtManager(int gpio, int dhtType);
    ~DhtManager();
    float getHumidity();
    float getTemperatureCelsius();
    float getTemperatureFahrenheit();
    void update();    

private:
    DHT *_dht;
    DebugMessage _debugMessage;
    float _humidity=0, _celsius=0, _fahrenheit=0;
};
#endif