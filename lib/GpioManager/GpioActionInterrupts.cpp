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

  // Serial.println("I got an interrupt!");
  // Serial.println("gpioInterruptPin: " + String(_iparameters->gpioInterruptPin));
  int digitalActionTotalInt = String(_iparameters->espConfig->getDataStore()->getValue("action_digital_total")).toInt();
  // Serial.println("Total of digital actions: " + String(digitalActionTotalInt));
  // Serial.println("gpioInterruptPinStatus: " + String(_iparameters->gpioInterruptPinStatus));
  // Serial.println("gpioInterruptPinLastStatus: " + String(_iparameters->gpioInterruptPinLastStatus));
  // Serial.println("GPIO interruptLastTimeInMillis: " + String(_iparameters->interruptLastTimeInMillis));
  String millisNow = String(millis());
  // Serial.println("millisNow: " + String(millisNow));
  for (int cont = 0 ; cont < digitalActionTotalInt ; cont ++)
  {
    String digitalGpioOrigin = "action_digital_gpio_origin_r_" + String(cont);
    int gpioInterruptPinLocal = String(_iparameters->espConfig->getDataStore()->getValue(digitalGpioOrigin.c_str())).toInt();
    if (gpioInterruptPinLocal == (int) _iparameters->gpioInterruptPin)
    {
      actionFound = true;
      long int interruptLastTimeinMillisParametersLongInt = atol(_iparameters->interruptLastTimeinMillisParameters[cont]->getValue());
      // Serial.println("_iparameters->interruptLastTimeinMillisParameters[cont]: " + String(interruptLastTimeinMillisParametersLongInt));
      String actionDigitalWaitingTimeRearm = "action_digital_waiting_time_rearm_r_" + String(cont);
      int digitalWaitingTimeRearm = String(_iparameters->espConfig->getDataStore()->getValue(actionDigitalWaitingTimeRearm.c_str())).toInt();
      // Serial.println("digitalWaitingTimeRearm: " + actionDigitalWaitingTimeRearm + " : " + String(digitalWaitingTimeRearm));

      String actionDigitalTriggerType = "action_digital_trigger_analisis_type_r_" + String(cont);
      String digitalTriggerType = String(_iparameters->espConfig->getDataStore()->getValue(actionDigitalTriggerType.c_str()));
      String actionDigitalAction = "action_digital_action_r_" + String(cont);
      String digitalAction = String(_iparameters->espConfig->getDataStore()->getValue(actionDigitalAction.c_str()));
      String actionDigitalGpioTarget = "action_digital_gpio_target_r_" + String(cont);
      String digitalGpioTarget = String(_iparameters->espConfig->getDataStore()->getValue(actionDigitalGpioTarget.c_str()));
      String actionDigitalTelegramMessage = "action_digital_telegram_message_r_" + String(cont);
      String digitalTelegramMessage = String(_iparameters->espConfig->getDataStore()->getValue(actionDigitalTelegramMessage.c_str()));
      // Serial.println("digitalTriggerType: " + actionDigitalTriggerType + " : " + digitalTriggerType);
      // Serial.println("digitalAction: " + actionDigitalAction + " : " + digitalAction);
      // Serial.println("digitalGpioTarget: " + actionDigitalGpioTarget + " : " + digitalGpioTarget);
      // Serial.println("digitalTelegramMessage: " + actionDigitalTelegramMessage + " : " + digitalTelegramMessage);
      // Trigger system must br improved to support per action 
      //bool triggerArmed = false;
      unsigned long timeSinceLastAction = ((millis() - interruptLastTimeinMillisParametersLongInt));
      // Serial.println("timeSinceLastAction: " + String(timeSinceLastAction));
      bool triggerArmed = false;
      if (timeSinceLastAction > (unsigned long) digitalWaitingTimeRearm)
      {
        triggerArmed = true;
      }
      if (triggerArmed)
      {
        if (((digitalTriggerType == "on") and (_iparameters->gpioInterruptPinStatus == 0)) or
            ((digitalTriggerType == "off") and (_iparameters->gpioInterruptPinStatus == 1)) or
            ((digitalTriggerType == "reversed") and (_iparameters->gpioInterruptPinStatus != _iparameters->gpioInterruptPinLastStatus)))
        {
          // Serial.println("Interrupt off will be processed");
          // Serial.println("Trigger Status: " + String(triggerArmed));
          executeDigitalAction(digitalAction, digitalGpioTarget.toInt(), digitalTelegramMessage);
          _iparameters->interruptLastTimeinMillisParameters[cont]->setValue(String(millisNow).c_str());
        }
        
        else
        {
          // Serial.println("Action " + String(cont) + " is not usable here");
        }
      }
      else
      {
        // Serial.println("Trigger is not armed for action " + String(cont));
      }
      // Serial.println();
    }
    else
    {
      // Serial.println("Action not found for id " + String(cont));
    }
  }
  _iparameters->interruptLastTimeInMillis = millis();
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

  if (action == "reverse")
  {
    // Serial.println("Reversing GPIO: " + String(gpioPinTarget));
    _iparameters->espConfig->setPinGpioStatusChanged(gpioPinTarget,1);
    _iparameters->espConfig->setPinGpioDigitalStatus(gpioPinTarget,!gpioManagerLocal->getDigitalOutput(gpioPinTarget));
    gpioManagerLocal->setDigitalOutput(gpioPinTarget, !gpioManagerLocal->getDigitalOutput(gpioPinTarget));
  }
  else if (action == "on")
  {
    // Serial.println("Turning on GPIO: " + String(gpioPinTarget));
    _iparameters->espConfig->setPinGpioStatusChanged(gpioPinTarget,1);
    _iparameters->espConfig->setPinGpioDigitalStatus(gpioPinTarget,HIGH);
    gpioManagerLocal->setDigitalOutput(gpioPinTarget, HIGH);
  }
  else if (action == "off")
  {
    // Serial.println("Turning off GPIO: " + String(gpioPinTarget));
    _iparameters->espConfig->setPinGpioStatusChanged(gpioPinTarget,1);
    _iparameters->espConfig->setPinGpioDigitalStatus(gpioPinTarget,LOW);
    gpioManagerLocal->setDigitalOutput(gpioPinTarget, LOW);
  }
  else if (action == "sendmessagetelegram")
  {
    // Serial.println("Sending Telegram message: " + telegramMessage);
  }
  else
  {
    return 0;
  }
  return 1;
}

