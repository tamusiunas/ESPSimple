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

// Parameters send to each ISR
struct interruptParameters 
{
   uint32_t gpioInterruptPin = -1; // GPIO pin
   int gpioInterruptPinStatus = -1; // GPIO Status (LOW/HIGH)
   int gpioInterruptPinLastStatus = -1; // GPIO Last (previous) Status (LOW/HIGH)
   bool isReconfigGpio = false; // check if it's a reconfig request (to access WEB Interface)
   //unsigned long interruptLastTimeInMillis = 0; // Last ISR Time in millis
   ESPConfig *espConfig = NULL; // ESP Object
   void *gpioManager; //it's an workaround because it's not able to create a GpioManager object. I will check soon (probably) if it's a Platformio issue.
   DataParameter **interruptLastTimeinMillisParameters = NULL; // Array for last ISR event time in millis
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
   volatile int *pinGpioPwmStatusChanged; 
   volatile int *pinGpioPwmStatus;
   volatile bool *pinPwmEnable;
   volatile int *pwmChannelGpioHw;
   volatile int *pwmChannelGpioSw;
   volatile int *sendAdcGpioValue;
   volatile int *sendAdcAnalogOnlyValue;
   volatile int *pinGpioAdcValue;
   volatile int *pinGpioAdcPreviousValue;
   volatile int *pinAnalogOnlyValue;
   volatile int *pinAnalogOnlyPreviousValue;
   volatile int *pinGpioAdcLastAction;
   volatile int *pinAnalogOnlyLastAction;

   volatile unsigned long *adcActionIndexLastTimeInMillis;
   volatile unsigned long *adcActionWhenReverseInMillis;

   volatile int totalGPIO;
   volatile int totalPwmSw; 
   volatile int totalPwmHw;
   volatile int totalDht;

   volatile bool *sendDhtCelsius ;
   volatile bool *sendDhtFahrenheit;
   volatile bool *sendDhtHumidity;

};

#endif