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

void AmazonAlexa::handle()
{
    _fauxmo->handle();
}

void AmazonAlexa::addDevice(const char *deviceName)
{
    Serial.println("Free Heap: " + String(ESP.getFreeHeap()));
    Serial.println("Adding device: " + String(deviceName));
    _fauxmo->addDevice(deviceName);
    Serial.println("Free Heap: " + String(ESP.getFreeHeap()));
}

void AmazonAlexa::addConfiguredDevices()
{
    int totalParameters = _alexaStruct->espConfig->getDataStore()->getDataParametersCount();
    for (int cont = 0 ; cont < totalParameters ; cont++)
    {
        String keyLocal = String(_alexaStruct->espConfig->getDataStore()->getParameters()[cont]->getField());
        if (keyLocal.indexOf("alexa_device_name_r_") == 0)
        {
            Serial.println("Found: " + keyLocal);
            addDevice(_alexaStruct->espConfig->getDataStore()->getParameters()[cont]->getValue());
        }
        //const char *deviceName = 
    }
    //alexa_device_name_r_
}

void AmazonAlexa::enable()
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
        // bool deviceFound = false;
        bool isDimmer = false;
        int gpioTarget = -1;
        String alexaDeviceNameStr = "alexa_device_name_r_" + String(device_id);
        String alexaSupportDimmerStr = "alexa_support_dimmer_r_" + String(device_id);
        String alexaGpioTargetStr = "alexa_gpio_target_r_" + String(device_id);
        if (strcmp(alexaStructLocal->espConfig->getDataStore()->getValue(alexaDeviceNameStr.c_str()),"") != 0)
        {
            Serial.println("ID: " + String(device_id));
            if (strcmp(alexaStructLocal->espConfig->getDataStore()->getValue(alexaSupportDimmerStr.c_str()),"yes") == 0)
            {
                Serial.println("It's a dimmer");
                isDimmer = true;
            }
            else
            {
                Serial.println("It's not a dimmer");
            }
            if (strcmp(alexaStructLocal->espConfig->getDataStore()->getValue(alexaGpioTargetStr.c_str()),"") != 0)
            {
                gpioTarget = String(alexaStructLocal->espConfig->getDataStore()->getValue(alexaGpioTargetStr.c_str())).toInt();
                Serial.println("Target: " + String(gpioTarget));
                if (isDimmer)
                {
                    if (state)
                    {
                        Serial.println("Setting Pwm");
                        alexaStructLocal->gpioManager->setPwm(gpioTarget,(int) (value * 4), pwmAdcDataLocal);
                    }
                    else
                    {
                        Serial.println("Setting Pwm to zero");
                        alexaStructLocal->gpioManager->setPwm(gpioTarget,0 , pwmAdcDataLocal);
                    }
                }
                else
                {
                    if (state)
                    {   Serial.println("Setting to High");
                        alexaStructLocal->gpioManager->setDigitalOutput(gpioTarget,HIGH);
                    }
                    else
                    {
                        Serial.println("Setting to Low");
                        alexaStructLocal->gpioManager->setDigitalOutput(gpioTarget,LOW);
                    }
                }
            } 
        } 

        //alexaStructLocal->gpioManager->setPwm(13,(int) (value * 4), pwmAdcDataLocal);
        //alexaStructLocal->gpioManager->setPwm(5,(int) (value * 4), pwmAdcDataLocal);
        //alexaStructLocal->gpioManager->setPwm(23,(int) (value * 4), pwmAdcDataLocal);
    }); 
}

void AmazonAlexa::disable()
{
    _fauxmo->enable(false, _alexaStruct,_pwmAdcData);
}
