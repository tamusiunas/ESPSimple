#include "AmazonAlexa.h"

AmazonAlexa::AmazonAlexa(AlexaStruct *alexaStruct, volatile PwmAdcData *pwmAdcData, unsigned long tcpPort)
{
    _fauxmo = new fauxmoESP();
    _fauxmo->createServer(true);
    _fauxmo->setPort(tcpPort);
    _alexaStruct = alexaStruct;
    _pwmAdcData = pwmAdcData;
    _debugMessage = DebugMessage();
}

AmazonAlexa::AmazonAlexa(AlexaStruct *alexaStruct, volatile PwmAdcData *pwmAdcData)
{
    _fauxmo = new fauxmoESP();
    _fauxmo->createServer(true);
    _fauxmo->setPort(80);
    _alexaStruct = alexaStruct;
    _pwmAdcData = pwmAdcData;
    _debugMessage = DebugMessage();
}

AmazonAlexa::~AmazonAlexa()
{
    delete _fauxmo;
}

void AmazonAlexa::handle()
{
    _fauxmo->handle();
}

void AmazonAlexa::addDevice(const char *deviceName)
{
    _debugMessage.debug("Adding Alexa device: " + String(deviceName));
    _fauxmo->addDevice(deviceName);
    _debugMessage.debug("Free Heap: " + String(ESP.getFreeHeap()));
}

void AmazonAlexa::addConfiguredDevices()
{
    int totalParameters = _alexaStruct->espConfig->getDataStore()->getDataParametersCount();
    for (int cont = 0 ; cont < totalParameters ; cont++)
    {
        String keyLocal = String(_alexaStruct->espConfig->getDataStore()->getParameters()[cont]->getField());
        if (keyLocal.indexOf("alexa_device_name_r_") == 0)
        {
            _debugMessage.debug("Found Alexa device config: " + keyLocal);
            addDevice(_alexaStruct->espConfig->getDataStore()->getParameters()[cont]->getValue());
        }
    }
}

void AmazonAlexa::enable()
{
    _fauxmo->enable(true, _alexaStruct, _pwmAdcData);
    _fauxmo->onSetState([](unsigned char deviceId, const char * deviceName, bool state, unsigned char value, void *arg1, volatile void *arg2) 
    {
        DebugMessage debugMessageLocal = DebugMessage();
        AlexaStruct *alexaStructLocal = (AlexaStruct *) arg1;
        volatile PwmAdcData *pwmAdcDataLocal = (volatile PwmAdcData *) arg2;
        debugMessageLocal.debug("Receiving Alexa Request - Device #" + String(deviceId) + " (" + deviceName + ") state: " + String(state) + " - value: " + String(value));
        bool isDimmer = false;
        int gpioTarget = -1;
        String alexaDeviceNameStr = "alexa_device_name_r_" + String(deviceId);
        String alexaSupportDimmerStr = "alexa_support_dimmer_r_" + String(deviceId);
        String alexaGpioTargetStr = "alexa_gpio_target_r_" + String(deviceId);
        if (strcmp(alexaStructLocal->espConfig->getDataStore()->getValue(alexaDeviceNameStr.c_str()),"") != 0)
        {
            debugMessageLocal.debug("Received Alexa request for device " + String(deviceId));
            if (strcmp(alexaStructLocal->espConfig->getDataStore()->getValue(alexaSupportDimmerStr.c_str()),"yes") == 0)
            {
                debugMessageLocal.debug("Alexa Request is for a dimmer");
                isDimmer = true;
            }
            else
            {
                debugMessageLocal.debug("Alexa Request is not for a dimmer");
            }
            if (strcmp(alexaStructLocal->espConfig->getDataStore()->getValue(alexaGpioTargetStr.c_str()),"") != 0)
            {
                gpioTarget = String(alexaStructLocal->espConfig->getDataStore()->getValue(alexaGpioTargetStr.c_str())).toInt();
                debugMessageLocal.debug("Alexa target: " + String(gpioTarget));
                if (isDimmer)
                {
                    if (state)
                    {
                        debugMessageLocal.debug("Alexa - Setting Pwm for " + String(gpioTarget) + " to " + String(value * 4));
                        alexaStructLocal->gpioManager->setPwm(gpioTarget,(int) (value * 4), pwmAdcDataLocal);
                    }
                    else
                    {
                        debugMessageLocal.debug("Alexa - Setting Pwm for " + String(gpioTarget) + " to zero");
                        alexaStructLocal->gpioManager->setPwm(gpioTarget,0 , pwmAdcDataLocal);
                    }
                }
                else
                {
                    if (state)
                    {   
                        debugMessageLocal.debug("Alexa - Setting " + String(gpioTarget) + " to high");
                        alexaStructLocal->gpioManager->setDigitalOutput(gpioTarget,HIGH);
                    }
                    else
                    {
                        debugMessageLocal.debug("Alexa - Setting " + String(gpioTarget) + " to low");
                        alexaStructLocal->gpioManager->setDigitalOutput(gpioTarget,LOW);
                    }
                }
            } 
        } 

    }); 
}

void AmazonAlexa::disable()
{
    _fauxmo->enable(false, _alexaStruct,_pwmAdcData);
}
