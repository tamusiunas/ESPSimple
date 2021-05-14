#include "GpioActionInterrupts.h"

GpioActionInterrupts::GpioActionInterrupts(interruptParameters *iparameters)
{
  _iparameters = iparameters;
}

GpioActionInterrupts::~GpioActionInterrupts()
{
  
}

bool GpioActionInterrupts::processDigitalAction()
{

  bool actionFound = false;
  int digitalActionTotalInt = String(_iparameters->espConfig->getDataStore()->getValue("action_digital_total")).toInt();
  String millisNow = String(millis());
  for (int cont = 0 ; cont < digitalActionTotalInt ; cont ++)
  {
    String digitalGpioOrigin = "action_digital_gpio_origin_r_" + String(cont);
    int gpioInterruptPinLocal = String(_iparameters->espConfig->getDataStore()->getValue(digitalGpioOrigin.c_str())).toInt();
    int gpioInterruptPinEvent = (int)_iparameters->gpioInterruptPin;
    
    Serial.println("gpioInterruptPinEvent: " + String(gpioInterruptPinEvent));
    Serial.println("gpioInterruptPinLocal: " + String(gpioInterruptPinLocal));

    if (gpioInterruptPinLocal == gpioInterruptPinEvent)
    {
      actionFound = true;
      long int interruptLastTimeinMillisParametersLongInt = atol(_iparameters->interruptLastTimeinMillisParameters[cont]->getValue());
      String actionDigitalWaitingTimeRearm = "action_digital_waiting_time_rearm_r_" + String(cont);
      int digitalWaitingTimeRearm =  atoi(_iparameters->espConfig->getDataStore()->getValue(actionDigitalWaitingTimeRearm.c_str()));
      Serial.println("digitalWaitingTimeRearm: " + String(digitalWaitingTimeRearm));
      String actionDigitalTriggerType = "action_digital_trigger_analisis_type_r_" + String(cont);
      String digitalTriggerType = String(_iparameters->espConfig->getDataStore()->getValue(actionDigitalTriggerType.c_str()));
      Serial.println("digitalTriggerType: " + String(digitalTriggerType));
      String actionDigitalAction = "action_digital_action_r_" + String(cont);
      String digitalAction = String(_iparameters->espConfig->getDataStore()->getValue(actionDigitalAction.c_str()));
      Serial.println("digitalAction: " + String(digitalAction));
      String actionDigitalGpioTarget = "action_digital_gpio_target_r_" + String(cont);
      String digitalGpioTarget = String(_iparameters->espConfig->getDataStore()->getValue(actionDigitalGpioTarget.c_str()));
      Serial.println("digitalGpioTarget: " + String(digitalGpioTarget));
      String actionDigitalTelegramMessage = "action_digital_telegram_message_r_" + String(cont);
      String digitalTelegramMessage = String(_iparameters->espConfig->getDataStore()->getValue(actionDigitalTelegramMessage.c_str()));
      Serial.println("digitalTelegramMessage: " + String(digitalTelegramMessage));
      unsigned long timeSinceLastAction = ((millis() - interruptLastTimeinMillisParametersLongInt));
      // Check for trigger armed
      if (timeSinceLastAction > (unsigned long) digitalWaitingTimeRearm)
      {       
        int gpioInterruptPinStatus = (int) _iparameters->gpioInterruptPinStatus;
        int gpioInterruptPinLastStatus = (int) _iparameters->gpioInterruptPinLastStatus;

        Serial.println("gpioInterruptPinStatus: " + String(gpioInterruptPinStatus));
        Serial.println("gpioInterruptPinLastStatus: " + String(gpioInterruptPinLastStatus));

        if (((digitalTriggerType == "on") and (gpioInterruptPinStatus == LOW)) or
            ((digitalTriggerType == "off") and (gpioInterruptPinStatus == HIGH)) or
            (digitalTriggerType == "reversed"))
        {
          if (_iparameters->espConfig->getPinPwmEnable()[digitalGpioTarget.toInt()] == false)
          {
            executeDigitalAction(digitalAction, digitalGpioTarget.toInt(), digitalTelegramMessage);
            _iparameters->interruptLastTimeinMillisParameters[cont]->setValue(String(millisNow).c_str());
            String actionDigitalTimeBeforeActionReversalStr = "action_digital_time_before_action_reversal_r_" + String(cont);
            int actionDigitalTimeBeforeActionReversalValueInt = String(_iparameters->espConfig->getDataStore()->getValue(actionDigitalTimeBeforeActionReversalStr.c_str())).toInt();
            if (actionDigitalTimeBeforeActionReversalValueInt > 0)
            {
              //Serial.println("Configuring getGpioDigitalActionIndexWhenReverseInMillis()[" + String(cont) + "] to " + 
              //  String(millis() + actionDigitalTimeBeforeActionReversalValueInt));
              _iparameters->espConfig->getGpioDigitalActionIndexWhenReverseInMillis()[cont] = millis() + actionDigitalTimeBeforeActionReversalValueInt;
            }
          }
          else
          {
            //Serial.println("GPIO " + digitalGpioTarget + " is set as Pwm not Output");
          }
          
        }
        else
        {
          Serial.println("Interrupt " + digitalTriggerType + " is not usable here");
        }
      }
      else
      {
        // Serial.println("Trigger is not armed for action " + String(cont));
      }
    }
    else
    {
    }
  }
  //_iparameters->interruptLastTimeInMillis = millis();
  _iparameters->gpioInterruptPinLastStatus = _iparameters->gpioInterruptPinStatus;
  if (actionFound)
  {
    return true;
  }
  else
  {
    return false;
  }
}

bool GpioActionInterrupts::executeDigitalAction(String action, int gpioPinTarget, String telegramMessage)
{
  GpioManager *gpioManagerLocal = (GpioManager *)_iparameters->gpioManager;
  Serial.println("executeDigitalAction:action: " + String(action));
  boolean configureGpio = false;
  uint32_t gpioValue = gpioManagerLocal->getDigitalOutput(gpioPinTarget);
  uint32_t gpioNewValue = 0;
  if (action == "reverse")
  {
    gpioNewValue = !gpioValue;
    configureGpio = true;
  }
  else if (action == "on")
  {
    gpioNewValue = HIGH;
    configureGpio = true;
  }
  else if (action == "off")
  {
    gpioNewValue = LOW;
    configureGpio = true;
  }
  #ifdef ENABLE_TELEGRAM
  else if (action == "sendmessagetelegram")
  {
    // Serial.println("Sending Telegram message: " + telegramMessage);
  }
  #endif
  else
  {
    return false;
  }
  if (configureGpio)
  {
    Serial.println("executeDigitalAction:gpioValue: " + String(gpioValue));
    Serial.println("executeDigitalAction:gpioNewValue: " + String(gpioNewValue));
    _iparameters->espConfig->setPinGpioDigitalStatusChanged(gpioPinTarget,1);
    _iparameters->espConfig->setPinGpioDigitalStatus(gpioPinTarget,gpioNewValue);
    gpioManagerLocal->setDigitalOutput(gpioPinTarget, gpioNewValue);
  }
  return true;
}

