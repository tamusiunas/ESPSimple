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
    if (gpioInterruptPinLocal == (int) _iparameters->gpioInterruptPin)
    {
      actionFound = true;
      long int interruptLastTimeinMillisParametersLongInt = atol(_iparameters->interruptLastTimeinMillisParameters[cont]->getValue());
      String actionDigitalWaitingTimeRearm = "action_digital_waiting_time_rearm_r_" + String(cont);
      int digitalWaitingTimeRearm = String(_iparameters->espConfig->getDataStore()->getValue(actionDigitalWaitingTimeRearm.c_str())).toInt();

      String actionDigitalTriggerType = "action_digital_trigger_analisis_type_r_" + String(cont);
      String digitalTriggerType = String(_iparameters->espConfig->getDataStore()->getValue(actionDigitalTriggerType.c_str()));
      String actionDigitalAction = "action_digital_action_r_" + String(cont);
      String digitalAction = String(_iparameters->espConfig->getDataStore()->getValue(actionDigitalAction.c_str()));
      String actionDigitalGpioTarget = "action_digital_gpio_target_r_" + String(cont);
      String digitalGpioTarget = String(_iparameters->espConfig->getDataStore()->getValue(actionDigitalGpioTarget.c_str()));
      String actionDigitalTelegramMessage = "action_digital_telegram_message_r_" + String(cont);
      String digitalTelegramMessage = String(_iparameters->espConfig->getDataStore()->getValue(actionDigitalTelegramMessage.c_str()));
      unsigned long timeSinceLastAction = ((millis() - interruptLastTimeinMillisParametersLongInt));
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
          if (_iparameters->espConfig->getPinPwmEnable()[digitalGpioTarget.toInt()] == false)
          {
            executeDigitalAction(digitalAction, digitalGpioTarget.toInt(), digitalTelegramMessage);
            _iparameters->interruptLastTimeinMillisParameters[cont]->setValue(String(millisNow).c_str());
          }
          else
          {
            Serial.println("GPIO " + digitalGpioTarget + " is set as Pwm not Output");
          }
          
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
    }
    else
    {
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
    Serial.println("Reversing GPIO: " + String(gpioPinTarget));
    _iparameters->espConfig->setPinGpioDigitalStatusChanged(gpioPinTarget,1);
    _iparameters->espConfig->setPinGpioDigitalStatus(gpioPinTarget,!gpioManagerLocal->getDigitalOutput(gpioPinTarget));
    gpioManagerLocal->setDigitalOutput(gpioPinTarget, !gpioManagerLocal->getDigitalOutput(gpioPinTarget));
  }
  else if (action == "on")
  {
    Serial.println("Turning on GPIO: " + String(gpioPinTarget));
    _iparameters->espConfig->setPinGpioDigitalStatusChanged(gpioPinTarget,1);
    _iparameters->espConfig->setPinGpioDigitalStatus(gpioPinTarget,HIGH);
    gpioManagerLocal->setDigitalOutput(gpioPinTarget, HIGH);
  }
  else if (action == "off")
  {
    Serial.println("Turning off GPIO: " + String(gpioPinTarget));
    _iparameters->espConfig->setPinGpioDigitalStatusChanged(gpioPinTarget,1);
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

