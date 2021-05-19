/**
 * @file CheckActionMqtt.h
 * @author Fabricio Tamusiunas (fabricio@tamusiunas.com)
 * @brief Headers for CheckActionMqtt.cpp
 * @version 0.1
 * @date 2019-02-11
 * 
 * @copyright Copyright (c) 2019
 * 
 */

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