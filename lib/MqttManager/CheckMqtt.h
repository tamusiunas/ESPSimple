#ifndef CheckMqtt_h
#define CheckMqtt_h

#include "MqttManagerOut.h"
#include "GpioCommons.h"
#include "DebugMessage.h"
#include "DhtManager.h"

class CheckMqtt
{
public:
    CheckMqtt(MqttManagerOut *mqttManagerOut, volatile PwmAdcData *pwmAdcDataLocal, DebugMessage *debugMessage);
    ~CheckMqtt();
    void checkDht(DhtManager **dhtManagerArray);
private:
    MqttManagerOut *_mqttManagerOut;
    volatile PwmAdcData *_pwmAdcDataLocal;
    DebugMessage *_debugMessage;
};

#endif