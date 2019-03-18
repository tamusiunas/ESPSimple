/**
 * @file GpioInterruptESP32.cpp
 * @author Fabricio Tamusiunas (fabricio@tamusiunas.com)
 * @brief Interrupt methods for ESP32
 * @version 0.1
 * @date 2019-02-09
 * 
 * @copyright Copyright (c) 2019
 * 
 */

#ifdef ESP32

#include "GpioManager.h"

void GpioManager::setInterrupt(uint32_t gpioInterruptPin)
{
  _debugMessage->debug("Configuring interrupt for GPIO: " + String(gpioInterruptPin));
  gpio_num_t gpioReceived = (gpio_num_t) gpioInterruptPin;
  if (!interrupInitialized)
  {
    gpio_install_isr_service(ESP_INTR_FLAG_LOWMED);
    interrupInitialized = true;
  }
  gpio_set_intr_type(gpioReceived, GPIO_INTR_ANYEDGE);
  //gpio_set_pull_mode(gpioReceived, GPIO_FLOATING);

  // Pointer to struct to transport gpio status and ESPConfig to the handleInterruptrr
  volatile interruptParameters *iparameters = (interruptParameters *)malloc(sizeof(interruptParameters));

  iparameters->gpioInterruptPin = gpioInterruptPin;
  iparameters->gpioInterruptPinStatus = 1;
  iparameters->gpioInterruptPinLastStatus = 1;
  iparameters->interruptLastTimeInMillis = 0;
  iparameters->espConfig = _espConfig;
  iparameters->gpioManager = this;
  
  int digitalActionTotalInt = String(iparameters->espConfig->getDataStore()->getValue("action_digital_total")).toInt();
  iparameters->interruptLastTimeinMillisParameters = (DataParameter**)malloc(digitalActionTotalInt * sizeof(DataParameter*));
  for (int cont = 0 ; cont < digitalActionTotalInt ; cont++)
  {
    iparameters->interruptLastTimeinMillisParameters[cont] = new DataParameter(String(cont).c_str(),"0000000000");
  }
  gpio_intr_enable(gpioReceived);
  gpio_isr_handler_add(gpioReceived, handleInterrupt, (void *) iparameters);
  
  // DO NOT FREE iparameters. If you do it the handleInterrupt will read trash!
  //free(iparameters);
}

void IRAM_ATTR GpioManager::handleInterrupt(void* arg)
{  
  interruptParameters *iparameters = (interruptParameters *) arg;

  int gpioInterruptStatus = digitalRead(iparameters->gpioInterruptPin);

  iparameters->gpioInterruptPinStatus = gpioInterruptStatus;

  GpioActionInterrupts gpioActionInterrupts = GpioActionInterrupts(iparameters);

  bool digitalActionStatus = gpioActionInterrupts.processDigitalAction();

  portYIELD_FROM_ISR();
}

#endif
