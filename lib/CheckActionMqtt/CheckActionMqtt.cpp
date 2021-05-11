#include "CheckActionMqtt.h"

CheckActionMqtt::CheckActionMqtt(MqttManagerOut *mqttManagerOut, volatile PwmAdcData *pwmAdcDataLocal, GpioManager *gpioManager, ESPConfig *espConfig)
{
    _mqttManagerOut = mqttManagerOut;
    _pwmAdcDataLocal = pwmAdcDataLocal;
    _debugMessage = DebugMessage();
    _gpioManager = gpioManager;
    _espConfig = espConfig;
}

CheckActionMqtt::~CheckActionMqtt()
{
  delete _mqttManagerOut;
  delete _pwmAdcDataLocal;
  delete _gpioManager;
  delete _espConfig;
}

void CheckActionMqtt::checkDht(DhtManager **dhtManagerArray)
{
  for (int cont = 0 ; cont < _pwmAdcDataLocal->totalDht ; cont++)
  {
    String mqttKey[3];
    String mqttValue[3];
    mqttKey[0] = "dhtId";
    mqttKey[1] = "key";
    mqttKey[2] = "value";
    mqttValue[0] = String(cont);
    if (_pwmAdcDataLocal->sendDhtHumidity[cont])
    {
      mqttValue[1] = "humidity";
      mqttValue[2] = String(dhtManagerArray[cont]->getHumidity());
      _pwmAdcDataLocal->sendDhtHumidity[cont] = false;
      _mqttManagerOut->publishMessageJson(mqttKey, mqttValue, 3, "InfoDht");
      _debugMessage.debug("Sent " + mqttValue[1] + " for DHT ID " + String(cont));
    }
    if (_pwmAdcDataLocal->sendDhtCelsius[cont])
    {
      mqttValue[1] = "celsius";
      mqttValue[2] = String(dhtManagerArray[cont]->getTemperatureCelsius());
      _pwmAdcDataLocal->sendDhtCelsius[cont] = false;
      _mqttManagerOut->publishMessageJson(mqttKey, mqttValue, 3, "InfoDht");
      _debugMessage.debug("Sent " + mqttValue[1] + " for DHT ID " + String(cont));
    }
    if (_pwmAdcDataLocal->sendDhtFahrenheit[cont])
    {
      mqttValue[1] = "fahrenheit";
      mqttValue[2] = String(dhtManagerArray[cont]->getTemperatureFahrenheit());
      _pwmAdcDataLocal->sendDhtFahrenheit[cont] = false;
      _mqttManagerOut->publishMessageJson(mqttKey, mqttValue, 3, "InfoDht");
      _debugMessage.debug("Sent " + mqttValue[1] + " for DHT ID " + String(cont));
    }
  }
}

void CheckActionMqtt::checkAdcReverse()
{
  int adcTotalInt = String(_espConfig->getDataStore()->getValue("action_adc_total")).toInt();
  for (int cont = 0 ; cont < adcTotalInt; cont++)
  {
    if ((_pwmAdcDataLocal->adcActionWhenReverseInMillis[cont] > 0) and (millis() > _pwmAdcDataLocal->adcActionWhenReverseInMillis[cont]))
    {
      _debugMessage.debug("Reversing ADC Action " + String(cont));
      _pwmAdcDataLocal->adcActionWhenReverseInMillis[cont] = 0;

      String actionAdcActionStr = "action_adc_action_r_" + String(cont);
      String actionAdcActionValueStr = String(_espConfig->getDataStore()->getValue(actionAdcActionStr.c_str()));
      String actionAdcGpioTargetStr = "action_adc_gpio_target_r_" + String(cont);
      int actionAdcGpioTargetValueInt = String(_espConfig->getDataStore()->getValue(actionAdcGpioTargetStr.c_str())).toInt();

      if (actionAdcActionValueStr == "reverse")
      {
        _debugMessage.debug("Reversing GPIO " + String(actionAdcGpioTargetValueInt));
        int gpioStatus = _gpioManager->getDigitalOutput(actionAdcGpioTargetValueInt);
        _gpioManager->setDigitalOutput(actionAdcGpioTargetValueInt, !gpioStatus);
        _espConfig->setPinGpioDigitalStatusChanged(actionAdcGpioTargetValueInt,1);
        _espConfig->setPinGpioDigitalStatus(actionAdcGpioTargetValueInt,!gpioStatus);
      }
      else if (actionAdcActionValueStr == "on")
      {
        _debugMessage.debug("Setting GPIO " + String(actionAdcGpioTargetValueInt) + " to LOW");
        _gpioManager->setDigitalOutput(actionAdcGpioTargetValueInt, LOW);
        _espConfig->setPinGpioDigitalStatusChanged(actionAdcGpioTargetValueInt,1);
        _espConfig->setPinGpioDigitalStatus(actionAdcGpioTargetValueInt,LOW);
      }
      else if (actionAdcActionValueStr == "off")
      {
        _debugMessage.debug("Setting GPIO " + String(actionAdcGpioTargetValueInt) + " to HIGH");
        _gpioManager->setDigitalOutput(actionAdcGpioTargetValueInt, HIGH);
        _espConfig->setPinGpioDigitalStatusChanged(actionAdcGpioTargetValueInt,1);
        _espConfig->setPinGpioDigitalStatus(actionAdcGpioTargetValueInt,HIGH);
      }

    }
  }
}

void CheckActionMqtt::checkGpioDigitalReverse()
{
  int gpioDigitalTotalInt = String(_espConfig->getDataStore()->getValue("action_digital_total")).toInt();
  for (int cont = 0 ; cont < gpioDigitalTotalInt; cont++)
  {
    if ((_espConfig->getGpioDigitalActionIndexWhenReverseInMillis()[cont] > 0) and (millis() > _espConfig->getGpioDigitalActionIndexWhenReverseInMillis()[cont]))
    {
      _debugMessage.debug("Reversing Gpio Digital Action " + String(cont));
      _espConfig->getGpioDigitalActionIndexWhenReverseInMillis()[cont] = 0;

      String actionGpioDigitalActionStr = "action_digital_action_r_" + String(cont);
      String actionGpioDigitalActionValueStr = String(_espConfig->getDataStore()->getValue(actionGpioDigitalActionStr.c_str()));
      String actionGpioDigitalGpioTargetStr = "action_digital_gpio_target_r_" + String(cont);
      int actionGpioDigitalGpioTargetValueInt = String(_espConfig->getDataStore()->getValue(actionGpioDigitalGpioTargetStr.c_str())).toInt();

      if (actionGpioDigitalActionValueStr == "reverse")
      {
        _debugMessage.debug("Reversing GPIO " + String(actionGpioDigitalGpioTargetValueInt));
        int gpioStatus = _gpioManager->getDigitalOutput(actionGpioDigitalGpioTargetValueInt);
        _gpioManager->setDigitalOutput(actionGpioDigitalGpioTargetValueInt, !gpioStatus);
        _espConfig->setPinGpioDigitalStatusChanged(actionGpioDigitalGpioTargetValueInt,1);
        _espConfig->setPinGpioDigitalStatus(actionGpioDigitalGpioTargetValueInt,!gpioStatus);
      }
      else if (actionGpioDigitalActionValueStr == "on")
      {
        _debugMessage.debug("Setting GPIO " + String(actionGpioDigitalGpioTargetValueInt) + " to LOW");
        _gpioManager->setDigitalOutput(actionGpioDigitalGpioTargetValueInt, LOW);
        _espConfig->setPinGpioDigitalStatusChanged(actionGpioDigitalGpioTargetValueInt,1);
        _espConfig->setPinGpioDigitalStatus(actionGpioDigitalGpioTargetValueInt,LOW);
      }
      else if (actionGpioDigitalActionValueStr == "off")
      {
        _debugMessage.debug("Setting GPIO " + String(actionGpioDigitalGpioTargetValueInt) + " to HIGH");
        _gpioManager->setDigitalOutput(actionGpioDigitalGpioTargetValueInt, HIGH);
        _espConfig->setPinGpioDigitalStatusChanged(actionGpioDigitalGpioTargetValueInt,1);
        _espConfig->setPinGpioDigitalStatus(actionGpioDigitalGpioTargetValueInt,HIGH);
      }

    }
  }
}

void CheckActionMqtt::checkAdcGpioActions()
{
  int adcValueGpioAdcArray[_pwmAdcDataLocal->totalGPIO];
  int adcValueAnalogOnlyArray[_pwmAdcDataLocal->totalGPIO];
  for (int cont = 0 ; cont < _pwmAdcDataLocal->totalGPIO ; cont++)
  {
    adcValueGpioAdcArray[cont] = -1;
    adcValueAnalogOnlyArray[cont] = -1;
  }

  int actionAdcTotal = String(_espConfig->getDataStore()->getValue("action_adc_total")).toInt();
  for (int cont = 0 ; cont < actionAdcTotal ; cont++)
  {
    String adcGpioOriginStr = "action_adc_gpio_origin_r_" + String(cont);
    String adcGpioOriginValueStr = String(_espConfig->getDataStore()->getValue(adcGpioOriginStr.c_str()));
    if (adcGpioOriginValueStr.startsWith("a"))
    {
      int adcAnalogOnlyOriginInt = String(_espConfig->getDataStore()->getValue(adcGpioOriginStr.c_str())).substring(1).toInt();
      if (adcValueAnalogOnlyArray[adcAnalogOnlyOriginInt] == -1 )
      {
        int adcValue = _gpioManager->getAdcValue(adcGpioOriginValueStr);
        adcValueAnalogOnlyArray[adcAnalogOnlyOriginInt] = adcValue;
      }
    }
    else
    {
      int adcGpioOriginInt = String(_espConfig->getDataStore()->getValue(adcGpioOriginStr.c_str())).toInt();
      if (adcValueGpioAdcArray[adcGpioOriginInt] == -1 )
      {
        int adcValue = _gpioManager->getAdcValue(adcGpioOriginValueStr);
        adcValueGpioAdcArray[adcGpioOriginInt] = adcValue;
      }
    }
  }
  for (int cont = 0 ; cont < actionAdcTotal ; cont++)
  {
    bool runAction = false;
    bool analogOnly = false;
    bool triggerArmed = false;

    int adcGpioOriginInt = -1;
    String adcGpioOriginStr = "action_adc_gpio_origin_r_" + String(cont);
    String adcGpioOriginValueStr = String(_espConfig->getDataStore()->getValue(adcGpioOriginStr.c_str()));
    if (adcGpioOriginValueStr.startsWith("a"))
    {
      analogOnly = true;
      adcGpioOriginInt = String(_espConfig->getDataStore()->getValue(adcGpioOriginStr.c_str())).substring(1).toInt();
    }
    else
    {
      adcGpioOriginInt = String(_espConfig->getDataStore()->getValue(adcGpioOriginStr.c_str())).toInt();
    }
    
    String actionAdcTriggerAnalisisTypeStr = "action_adc_trigger_analisis_type_r_" + String(cont);
    String actionAdcTriggerAnalisisTypeValueStr = String(_espConfig->getDataStore()->getValue(actionAdcTriggerAnalisisTypeStr.c_str()));
    String actionAdcTriggerValueStr = "action_adc_trigger_value_r_" + String(cont);
    int actionAdcTriggerValueValueInt = String(_espConfig->getDataStore()->getValue(actionAdcTriggerValueStr.c_str())).toInt();
    String actionAdcWaitingTimeRearmStr = "action_adc_waiting_time_rearm_r_"+ String(cont);
    int actionAdcWaitingTimeRearmValueInt = String(_espConfig->getDataStore()->getValue(actionAdcWaitingTimeRearmStr.c_str())).toInt();

    if ((millis() - _pwmAdcDataLocal->adcActionIndexLastTimeInMillis[cont]) > (unsigned long) actionAdcWaitingTimeRearmValueInt)
    {
      triggerArmed = true;      
    }

    if (triggerArmed)
    {
      if ((analogOnly) and (_pwmAdcDataLocal->pinAnalogOnlyValue[adcGpioOriginInt] > -1))
      {
        int adcValue = adcValueAnalogOnlyArray[adcGpioOriginInt];
        if (actionAdcTriggerAnalisisTypeValueStr == "variation")
        {
          if (((adcValue - _pwmAdcDataLocal->pinAnalogOnlyValue[adcGpioOriginInt]) >= actionAdcTriggerValueValueInt) or
              ((adcValue - _pwmAdcDataLocal->pinAnalogOnlyValue[adcGpioOriginInt]) <= (actionAdcTriggerValueValueInt - (actionAdcTriggerValueValueInt * 2))))
          {
            runAction = true;
            _debugMessage.debug("It's an analog only variation action - Value: " + String(adcValue) + " - Diff: " + String(adcValue - _pwmAdcDataLocal->pinAnalogOnlyValue[adcGpioOriginInt]));
          }
        }
        else if (actionAdcTriggerAnalisisTypeValueStr == "greaterthan")
        {
          if ((adcValue > actionAdcTriggerValueValueInt) and (_pwmAdcDataLocal->pinAnalogOnlyValue[adcGpioOriginInt] <= actionAdcTriggerValueValueInt))
          {
            runAction = true;
            _debugMessage.debug("It's an analog only greaterthan action - Value: " + String(adcValue) + " - Previous Value: " + String(_pwmAdcDataLocal->pinAnalogOnlyValue[adcGpioOriginInt]));
          }
        }
        else if (actionAdcTriggerAnalisisTypeValueStr == "lowerthan")
        {
          if ((adcValue < actionAdcTriggerValueValueInt) and (_pwmAdcDataLocal->pinAnalogOnlyValue[adcGpioOriginInt] >= actionAdcTriggerValueValueInt))
          {
            runAction = true;
            _debugMessage.debug("It's an analog only lowerthan action - Value: " + String(adcValue) + " - Previous Value: " + String(_pwmAdcDataLocal->pinAnalogOnlyValue[adcGpioOriginInt]));
          }
        }
      }
      else if (_pwmAdcDataLocal->pinGpioAdcValue[adcGpioOriginInt] > -1)
      {
        int adcValue = adcValueGpioAdcArray[adcGpioOriginInt];
        if (actionAdcTriggerAnalisisTypeValueStr == "variation")
        {
          if (((adcValue - _pwmAdcDataLocal->pinGpioAdcValue[adcGpioOriginInt]) >= actionAdcTriggerValueValueInt) or
              ((adcValue - _pwmAdcDataLocal->pinGpioAdcValue[adcGpioOriginInt]) <= (actionAdcTriggerValueValueInt - (actionAdcTriggerValueValueInt * 2))))
          {
            runAction = true;
            _debugMessage.debug("It's an ADC GPIO variation action - Value: " + String(adcValue) + " - Diff: " + String(adcValue - _pwmAdcDataLocal->pinGpioAdcValue[adcGpioOriginInt]));
          }
        }
        else if (actionAdcTriggerAnalisisTypeValueStr == "greaterthan")
        {
          if ((adcValue > actionAdcTriggerValueValueInt) and (_pwmAdcDataLocal->pinGpioAdcValue[adcGpioOriginInt] <= actionAdcTriggerValueValueInt))
          {
            runAction = true;
            _debugMessage.debug("It's an ADC GPIO greaterthan action - Value: " + String(adcValue) + " - Previous Value: " + String(_pwmAdcDataLocal->pinGpioAdcValue[adcGpioOriginInt]));
          }
        }
        else if (actionAdcTriggerAnalisisTypeValueStr == "lowerthan")
        {
          if ((adcValue < actionAdcTriggerValueValueInt) and (_pwmAdcDataLocal->pinGpioAdcValue[adcGpioOriginInt] >= actionAdcTriggerValueValueInt))
          {
            runAction = true;
            _debugMessage.debug("It's an ADC GPIO lowerthan action - Value: " + String(adcValue) + " - Previous Value: " + String(_pwmAdcDataLocal->pinGpioAdcValue[adcGpioOriginInt]));
          }
        }  
      }
    }
    
    if (runAction)
    {
      runDigitalActions(cont);
    }
  } 

  // set the previous values arrays
  for (int cont = 0 ; cont < _pwmAdcDataLocal->totalGPIO ; cont++)
  {
    _pwmAdcDataLocal->pinGpioAdcPreviousValue[cont] = _pwmAdcDataLocal->pinGpioAdcValue[cont];
    _pwmAdcDataLocal->pinGpioAdcValue[cont] = adcValueGpioAdcArray[cont];
    _pwmAdcDataLocal->pinAnalogOnlyPreviousValue[cont] = _pwmAdcDataLocal->pinAnalogOnlyValue[cont];
    _pwmAdcDataLocal->pinAnalogOnlyValue[cont] = adcValueAnalogOnlyArray[cont];
  }
}

void CheckActionMqtt::runDigitalActions(int position)
{
    String actionAdcActionStr = "action_adc_action_r_" + String(position);
    String actionAdcActionValueStr = String(_espConfig->getDataStore()->getValue(actionAdcActionStr.c_str()));
    String actionAdcGpioTargetStr = "action_adc_gpio_target_r_" + String(position);
    int actionAdcGpioTargetValueInt = String(_espConfig->getDataStore()->getValue(actionAdcGpioTargetStr.c_str())).toInt();
    String actionAdcTimeBeforeActionReversal = "action_adc_time_before_action_reversal_r_" + String(position);
    int actionAdcTimeBeforeActionReversalValueInt = String(_espConfig->getDataStore()->getValue(actionAdcTimeBeforeActionReversal.c_str())).toInt();
    if (actionAdcActionValueStr == "reverse")
    {
    _debugMessage.debug("Reversing GPIO " + String(actionAdcGpioTargetValueInt));
    int gpioStatus = _gpioManager->getDigitalOutput(actionAdcGpioTargetValueInt);
    _gpioManager->setDigitalOutput(actionAdcGpioTargetValueInt, !gpioStatus);
    _espConfig->setPinGpioDigitalStatusChanged(actionAdcGpioTargetValueInt,1);
    _espConfig->setPinGpioDigitalStatus(actionAdcGpioTargetValueInt,!gpioStatus);
    }
    else if (actionAdcActionValueStr == "on")
    {
    _debugMessage.debug("Setting " + String(actionAdcGpioTargetValueInt) + " to HIGH");
    _gpioManager->setDigitalOutput(actionAdcGpioTargetValueInt, HIGH);
    _espConfig->setPinGpioDigitalStatusChanged(actionAdcGpioTargetValueInt,1);
    _espConfig->setPinGpioDigitalStatus(actionAdcGpioTargetValueInt,HIGH);
    }
    else if (actionAdcActionValueStr == "off")
    {
    _debugMessage.debug("Setting " + String(actionAdcGpioTargetValueInt) + " to LOW");
    _gpioManager->setDigitalOutput(actionAdcGpioTargetValueInt, LOW);
    _espConfig->setPinGpioDigitalStatusChanged(actionAdcGpioTargetValueInt,1);
    _espConfig->setPinGpioDigitalStatus(actionAdcGpioTargetValueInt,LOW);
    }
    if (actionAdcTimeBeforeActionReversalValueInt > 0)
    {
    _pwmAdcDataLocal->adcActionWhenReverseInMillis[position] = (millis() + actionAdcTimeBeforeActionReversalValueInt);
    }

    _pwmAdcDataLocal->adcActionIndexLastTimeInMillis[position] = millis();
}