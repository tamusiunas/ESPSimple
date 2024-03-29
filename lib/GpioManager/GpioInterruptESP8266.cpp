/**
 * @file GpioInterruptESP8266.cpp
 * @author Fabricio Tamusiunas (fabricio@tamusiunas.com)
 * @brief Interrupt methods for ESP8266
 * @version 0.1
 * @date 2019-02-09
 * 
 * @copyright Copyright (c) 2019
 * 
 */

#ifdef ESP8266

#include "GpioManager.h"

void GpioManager::setInterrupt(uint32_t gpioInterruptPin)
{

  // Pointer to struct to transport gpio status and ESPConfig to the handleInterruptrr
  volatile interruptParameters *iparameters = (interruptParameters *)malloc(sizeof(interruptParameters));

  iparameters->gpioInterruptPin = gpioInterruptPin;
  iparameters->gpioInterruptPinStatus = 1;
  iparameters->gpioInterruptPinLastStatus = 1;
  //iparameters->interruptLastTimeInMillis = 0;
  iparameters->espConfig = _espConfig;
  iparameters->gpioManager = this;
  
  #ifdef ENABLE_GPIO_WEB_CONFIG
  String webConfigGpioStr = String(_espConfig->getDataStore()->getValue("web_config_gpio"));
  if (webConfigGpioStr != "none")
  {
    int webConfigGpio = String(_espConfig->getDataStore()->getValue("web_config_gpio")).toInt();
    if (webConfigGpio == (int) gpioInterruptPin)
    {
      iparameters->isReconfigGpio = true;
    }
  }
  else
  {
    iparameters->isReconfigGpio = false;
  }
  #endif

  int digitalActionTotalInt = String(iparameters->espConfig->getDataStore()->getValue("action_digital_total")).toInt();
  iparameters->interruptLastTimeinMillisParameters = (DataParameter**)malloc(digitalActionTotalInt * sizeof(DataParameter*));
  for (int cont = 0 ; cont < digitalActionTotalInt ; cont++)
  {
    iparameters->interruptLastTimeinMillisParameters[cont] = new DataParameter(String(cont).c_str(),"0000000000");
  }

  //attachInterrupt(digitalPinToInterrupt(gpioInterruptPin), std::bind(&GpioManager::handleInterrupt,this,gpioInterruptPin), CHANGE);
  attachInterrupt(digitalPinToInterrupt(gpioInterruptPin), std::bind(&GpioManager::handleInterrupt,this,(void *)iparameters), CHANGE);

}

void ICACHE_RAM_ATTR GpioManager::handleInterrupt(void *arg)
{
  interruptParameters *iparameters = (interruptParameters *) arg;

  int gpioInterruptStatus = digitalRead(iparameters->gpioInterruptPin);

  #ifdef ENABLE_GPIO_WEB_CONFIG
  if ((iparameters->isReconfigGpio) and (gpioInterruptStatus == LOW))
  {
    DoubleReset doubleReset = DoubleReset();
    doubleReset.setResetValue(2);
    ESP.restart();
  }
  #endif 

  iparameters->gpioInterruptPinStatus = gpioInterruptStatus;

  GpioActionInterrupts gpioActionInterrupts = GpioActionInterrupts(iparameters);

  bool digitalActionStatus = gpioActionInterrupts.processDigitalAction();
}

#endif
