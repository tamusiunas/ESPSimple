/**
 * @file GpioActionInterrupts.h
 * @author Fabricio Tamusiunas (fabricio@tamusiunas.com)
 * @brief 
 * @version 0.1
 * @date 2019-02-19
 * 
 * @copyright Copyright (c) 2019
 * 
 */
#ifndef GpioActionInterrupts_h
#define GpioActionInterrupts_h

#include "ESPConfig.h"
#include "GpioManager.h"
#include "GpioCommons.h"


class GpioActionInterrupts
{
public:
  GpioActionInterrupts(interruptParameters *iparameters);
  ~GpioActionInterrupts();
  bool processDigitalAction();
  bool executeDigitalAction(String action, int gpioPinTarget, String telegramMessage);


private:
  interruptParameters *_iparameters;
};

#endif