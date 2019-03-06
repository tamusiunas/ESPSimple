#include "AmazonAlexa.h"

AmazonAlexa::AmazonAlexa(AlexaStruct *alexaStruct, volatile PwmAdcData *pwmAdcData, unsigned long tcpPort)
{
    _fauxmo = new fauxmoESP();
    _fauxmo->createServer(true);
    _fauxmo->setPort(tcpPort);
    _alexaStruct = alexaStruct;
    _pwmAdcData = pwmAdcData;
}

AmazonAlexa::AmazonAlexa(AlexaStruct *alexaStruct, volatile PwmAdcData *pwmAdcData)
{
    Serial.println("I0");
    _fauxmo = new fauxmoESP();
    Serial.println("I1");
    _fauxmo->createServer(true);
    Serial.println("I2");
    _fauxmo->setPort(80);
    Serial.println("I3");
    _alexaStruct = alexaStruct;
    Serial.println("I4");
    _pwmAdcData = pwmAdcData;
    Serial.println("I5");
}

AmazonAlexa::~AmazonAlexa()
{

}

void AmazonAlexa::Handle()
{
    _fauxmo->handle();
}

void AmazonAlexa::AddDevice(const char *deviceName)
{
    _fauxmo->addDevice(deviceName);
}

void AmazonAlexa::Enable()
{
    Serial.println("I1");
    _fauxmo->enable(true, _alexaStruct, _pwmAdcData);
    Serial.println("I2");
    _fauxmo->onSetState([](unsigned char device_id, const char * device_name, bool state, unsigned char value, void *arg1, volatile void *arg2) 
    {
        AlexaStruct *alexaStructLocal = (AlexaStruct *) arg1;
        volatile PwmAdcData *pwmAdcDataLocal = (volatile PwmAdcData *) arg2;
        //Serial.println("gpio_comment_26: " + String(alexaStructLocal->espConfig->getDataStore()->getValue("gpio_comment_26")));
        Serial.printf("[MAIN] Device #%d (%s) state: %s value: %d\n", device_id, device_name, state ? "ON" : "OFF", value);
        //alexaStructLocal->gpioManager->setPwm(13,(int) (value * 4), pwmAdcDataLocal);
        //alexaStructLocal->gpioManager->setPwm(5,(int) (value * 4), pwmAdcDataLocal);
        //alexaStructLocal->gpioManager->setPwm(23,(int) (value * 4), pwmAdcDataLocal);
    });
    
}

void AmazonAlexa::Disable()
{
    _fauxmo->enable(false, _alexaStruct,_pwmAdcData);
}
