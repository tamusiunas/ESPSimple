#ifndef CheckActionMqtt_h
#define CheckActionMqtt_h

#include "MqttManagerOut.h"
#include "GpioCommons.h"
#include "DebugMessage.h"
#include "DhtManager.h"
#include "GpioManager.h"
#include "ESPConfig.h"

class CheckActionMqtt
{
public:
    CheckActionMqtt(MqttManagerOut *mqttManagerOut, volatile PwmAdcData *pwmAdcDataLocal, GpioManager *gpioManager, ESPConfig *espConfig);
    ~CheckActionMqtt();
    void checkDht(DhtManager **dhtManagerArray);
    void checkAdcReverse();
    void checkGpioDigitalReverse();
    void checkAdcGpioActions();
private:
    MqttManagerOut *_mqttManagerOut;
    volatile PwmAdcData *_pwmAdcDataLocal;
    DebugMessage _debugMessage;
    GpioManager *_gpioManager;
    ESPConfig *_espConfig;
    void runDigitalActions(int position);
};

#endif