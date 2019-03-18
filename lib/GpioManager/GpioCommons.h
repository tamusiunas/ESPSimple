/**
 * @file GpioCommons.h
 * @author Fabricio Tamusiunas (fabricio@tamusiunas.com)
 * @brief Commons structs for Gpio Objects
 * @version 0.1
 * @date 2019-02-19
 * 
 * @copyright Copyright (c) 2019
 * 
 */

#ifndef GpioCommons_h
#define GpioCommons_h

#include "ESPConfig.h"

struct interruptParameters 
{
   uint32_t gpioInterruptPin = -1;
   int gpioInterruptPinStatus = -1;
   int gpioInterruptPinLastStatus = -1;
   unsigned long interruptLastTimeInMillis = 0;
   ESPConfig *espConfig = NULL;
   void *gpioManager; //it's an workaround because it's not able to create an GpioManager object. I will check soon (probably) if it's a Platformio issue.
   DataParameter **interruptLastTimeinMillisParameters = NULL;
};

struct mqttOutputQueueInterrupts
{
   bool isMqttQueueLocked = false; // check if queue is in use
   DataParameter **gpioDigitalStatus = NULL;
   DataParameter **gpioPwmStatus = NULL;
   int gpioDigitalCount = 0;
   int gpioPwmCount = 0;
};

struct PwmAdcData
{
   volatile int *pinGpioDigitalStatusChanged;
   volatile int *pinGpioDigitalStatus;
   volatile int *pinGpioAdcValue;
   volatile int *pinGpioAdcPreviousValue;
   volatile int *pinGpioPwmStatusChanged; 
   volatile int *pinGpioPwmStatus;
   volatile bool *pinPwmEnable;
   volatile int *pwmChannelGpioHw;
   volatile int *pwmChannelGpioSw;
   volatile int *sendAdcGpioValue;
   volatile int *sendAdcAnalogOnlyValue;
   volatile int *pinAnalogOnlyValue;
   volatile int *pinAnalogOnlyPreviousValue;

   volatile int *pinGpioAdcLastAction;
   volatile unsigned long *pinGpioAdcLastActionTime;
   volatile int *pinAnalogOnlyLastAction;
   volatile unsigned long *pinAnalogOnlyLastActionTime;

   volatile int totalGPIO;
   volatile int totalPwmSw; 
   volatile int totalPwmHw;

};

#endif