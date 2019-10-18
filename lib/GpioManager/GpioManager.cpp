/**
 * @file GpioManager.cpp
 * @author Fabricio Tamusiunas (fabricio@tamusiunas.com)
 * @brief Read and configure GPIO (mode and PWM)
 * @version 0.1
 * @date 2019-02-09
 * 
 * @copyright Copyright (c) 2019
 * 
 */
#include "GpioManager.h"

GpioManager::GpioManager(ESPConfig *espConfig, DebugMessage *debugMessage)
{
  _espConfig = espConfig;
  //_mqttManagerOut = NULL;
  initializeTimers();
  _debugMessage = debugMessage;
}

GpioManager::~GpioManager()
{

}

void GpioManager::initializeGpio()
{
  int totalParameters = _espConfig->getDataStore()->getDataParametersCount();
  _debugMessage->debug("Initializing GPIO");
  for (int cont=0 ; cont < totalParameters ; ++cont )
  {
    DataParameter *dataParameter = _espConfig->getDataStore()->getParameterByPos(cont);
    if (dataParameter != NULL)
    {
      if (String(dataParameter->getField()).indexOf("gpio_mode_") == 0)
      {
        int gpioInt = String(String(dataParameter->getField()).substring(10)).toInt();
        const char *gpioModeCChar = dataParameter->getValue();
        _debugMessage->debug("Initializing GPIO - GPIO : " + String(gpioInt) + " - Mode: " + String(gpioModeCChar));
        if (strcmp(gpioModeCChar,"input_pullup") == 0)
        {
          setGpioMode(gpioInt,INPUT_PULLUP);
          setInterrupt(gpioInt);
        }
        else
        if (strcmp(gpioModeCChar,"input") == 0)
        {
          setGpioMode(gpioInt,INPUT);
        }
        else
        if (strcmp(gpioModeCChar,"output") == 0)
        {
          setGpioMode(gpioInt,OUTPUT);
        }
        else
        if (strcmp(gpioModeCChar,"adc") == 0)
        {
          configAdc(String(gpioInt));
        }
      }
      else if (String(dataParameter->getField()).indexOf("gpio_adc_analog_only_") == 0)
      {
        int gpioAnalogInt = String(dataParameter->getField()).substring(21).toInt();
        String gpioString = "a" + String(gpioAnalogInt);
        configAdc(gpioString);
      }
      else
      if (String(dataParameter->getField()).indexOf("pwm_gpio_r_") == 0)
      {
        int pwmPosition = String(String(dataParameter->getField()).substring(11)).toInt();
        int PwmGpio = String(dataParameter->getValue()).toInt();
        String gpioSource = String(_espConfig->getDataStore()->getValue(String("pwm_source_r_" + String(pwmPosition)).c_str()));
        _debugMessage->debug("Initializing GPIO - GPIO " + String(PwmGpio) + " is configured as PWM.");
        if (gpioSource == "zerocross")
        {
          addGpioPwmZeroCross(PwmGpio);
        }
        else
        if (gpioSource == "nonzerocross")
        {
          addGpioPwmNonZeroCross(PwmGpio);
        }
        _espConfig->setPinPwmEnable(PwmGpio,true);
      }
    }
  }
}

void GpioManager::setGpioMode(int gpio, byte mode)
{
  pinMode(gpio, mode);
  _espConfig->setPinGpioMode(gpio,mode);
}

ESPConfig *GpioManager::getESPConfig()
{
  return _espConfig;
}

/*void GpioManager::checkAdcGpioActions(MqttManagerOut *mqttManagerOut, volatile PwmAdcData *pwmAdcDataLocal)
{
  int adcValueGpioAdcArray[pwmAdcDataLocal->totalGPIO];
  int adcValueAnalogOnlyArray[pwmAdcDataLocal->totalGPIO];
  for (int cont = 0 ; cont < pwmAdcDataLocal->totalGPIO ; cont++)
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
        int adcValue = getAdcValue(adcGpioOriginValueStr);
        adcValueAnalogOnlyArray[adcAnalogOnlyOriginInt] = adcValue;
        //_debugMessage->debug("adcValueGpipAnalogOnlyArray[" + String(adcAnalogOnlyOriginInt) + "] = " + String(adcValue));
      }
    }
    else
    {
      int adcGpioOriginInt = String(_espConfig->getDataStore()->getValue(adcGpioOriginStr.c_str())).toInt();
      if (adcValueGpioAdcArray[adcGpioOriginInt] == -1 )
      {
        int adcValue = getAdcValue(adcGpioOriginValueStr);
        adcValueGpioAdcArray[adcGpioOriginInt] = adcValue;
        //_debugMessage->debug("adcValueGpioAdcArray[" + String(adcGpioOriginInt) + "] = " + String(adcValue));
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

    if ((millis() - pwmAdcDataLocal->adcActionIndexLastTimeInMillis[cont]) > (unsigned long) actionAdcWaitingTimeRearmValueInt)
    {
      triggerArmed = true;      
      //_debugMessage->debug("Trigger " + String(cont) + " armed - adcActionIndexLastTimeInMillis: " + String(pwmAdcDataLocal->adcActionIndexLastTimeInMillis[cont]));
    }
    else
    {
      //_debugMessage->debug("Trigger " + String(cont) + " not armed - adcActionIndexLastTimeInMillis: " + String(pwmAdcDataLocal->adcActionIndexLastTimeInMillis[cont]));
    }

    if (triggerArmed)
    {
      if ((analogOnly) and (pwmAdcDataLocal->pinAnalogOnlyValue[adcGpioOriginInt] > -1))
      {
        int adcValue = adcValueAnalogOnlyArray[adcGpioOriginInt];
        if (actionAdcTriggerAnalisisTypeValueStr == "variation")
        {
          if (((adcValue - pwmAdcDataLocal->pinAnalogOnlyValue[adcGpioOriginInt]) >= actionAdcTriggerValueValueInt) or
              ((adcValue - pwmAdcDataLocal->pinAnalogOnlyValue[adcGpioOriginInt]) <= (actionAdcTriggerValueValueInt - (actionAdcTriggerValueValueInt * 2))))
          {
            runAction = true;
            _debugMessage->debug("It's an analog only variation action - Value: " + String(adcValue) + " - Diff: " + String(adcValue - pwmAdcDataLocal->pinAnalogOnlyValue[adcGpioOriginInt]));
          }
        }
        else if (actionAdcTriggerAnalisisTypeValueStr == "greaterthan")
        {
          if ((adcValue > actionAdcTriggerValueValueInt) and (pwmAdcDataLocal->pinAnalogOnlyValue[adcGpioOriginInt] <= actionAdcTriggerValueValueInt))
          {
            runAction = true;
            _debugMessage->debug("It's an analog only greaterthan action - Value: " + String(adcValue) + " - Previous Value: " + String(pwmAdcDataLocal->pinAnalogOnlyValue[adcGpioOriginInt]));
          }
        }
        else if (actionAdcTriggerAnalisisTypeValueStr == "lowerthan")
        {
          if ((adcValue < actionAdcTriggerValueValueInt) and (pwmAdcDataLocal->pinAnalogOnlyValue[adcGpioOriginInt] >= actionAdcTriggerValueValueInt))
          {
            runAction = true;
            _debugMessage->debug("It's an analog only lowerthan action - Value: " + String(adcValue) + " - Previous Value: " + String(pwmAdcDataLocal->pinAnalogOnlyValue[adcGpioOriginInt]));
          }
        }
      }
      else if (pwmAdcDataLocal->pinGpioAdcValue[adcGpioOriginInt] > -1)
      {
        int adcValue = adcValueGpioAdcArray[adcGpioOriginInt];
        if (actionAdcTriggerAnalisisTypeValueStr == "variation")
        {
          if (((adcValue - pwmAdcDataLocal->pinGpioAdcValue[adcGpioOriginInt]) >= actionAdcTriggerValueValueInt) or
              ((adcValue - pwmAdcDataLocal->pinGpioAdcValue[adcGpioOriginInt]) <= (actionAdcTriggerValueValueInt - (actionAdcTriggerValueValueInt * 2))))
          {
            runAction = true;
            _debugMessage->debug("It's an ADC GPIO variation action - Value: " + String(adcValue) + " - Diff: " + String(adcValue - pwmAdcDataLocal->pinGpioAdcValue[adcGpioOriginInt]));
          }
        }
        else if (actionAdcTriggerAnalisisTypeValueStr == "greaterthan")
        {
          if ((adcValue > actionAdcTriggerValueValueInt) and (pwmAdcDataLocal->pinGpioAdcValue[adcGpioOriginInt] <= actionAdcTriggerValueValueInt))
          {
            runAction = true;
            _debugMessage->debug("It's an ADC GPIO greaterthan action - Value: " + String(adcValue) + " - Previous Value: " + String(pwmAdcDataLocal->pinGpioAdcValue[adcGpioOriginInt]));
          }
        }
        else if (actionAdcTriggerAnalisisTypeValueStr == "lowerthan")
        {
          if ((adcValue < actionAdcTriggerValueValueInt) and (pwmAdcDataLocal->pinGpioAdcValue[adcGpioOriginInt] >= actionAdcTriggerValueValueInt))
          {
            runAction = true;
            _debugMessage->debug("It's an ADC GPIO lowerthan action - Value: " + String(adcValue) + " - Previous Value: " + String(pwmAdcDataLocal->pinGpioAdcValue[adcGpioOriginInt]));
          }
        }  
      }
    }
    
    if (runAction)
    {
      String actionAdcActionStr = "action_adc_action_r_" + String(cont);
      String actionAdcActionValueStr = String(_espConfig->getDataStore()->getValue(actionAdcActionStr.c_str()));
      String actionAdcGpioTargetStr = "action_adc_gpio_target_r_" + String(cont);
      int actionAdcGpioTargetValueInt = String(_espConfig->getDataStore()->getValue(actionAdcGpioTargetStr.c_str())).toInt();
      String actionAdcTimeBeforeActionReversal = "action_adc_time_before_action_reversal_r_" + String(cont);
      //_debugMessage->debug(actionAdcTimeBeforeActionReversal + ": " + String(_espConfig->getDataStore()->getValue(actionAdcTimeBeforeActionReversal.c_str())));
      int actionAdcTimeBeforeActionReversalValueInt = String(_espConfig->getDataStore()->getValue(actionAdcTimeBeforeActionReversal.c_str())).toInt();
      if (actionAdcActionValueStr == "reverse")
      {
        //pwmAdcDataLocal->adc
        _debugMessage->debug("Reversing GPIO " + String(actionAdcGpioTargetValueInt));
        int gpioStatus = getDigitalOutput(actionAdcGpioTargetValueInt);
        setDigitalOutput(actionAdcGpioTargetValueInt, !gpioStatus);
        _espConfig->setPinGpioDigitalStatusChanged(actionAdcGpioTargetValueInt,1);
        _espConfig->setPinGpioDigitalStatus(actionAdcGpioTargetValueInt,!gpioStatus);
      }
      else if (actionAdcActionValueStr == "on")
      {
        _debugMessage->debug("Setting " + String(actionAdcGpioTargetValueInt) + " to HIGH");
        setDigitalOutput(actionAdcGpioTargetValueInt, HIGH);
        _espConfig->setPinGpioDigitalStatusChanged(actionAdcGpioTargetValueInt,1);
        _espConfig->setPinGpioDigitalStatus(actionAdcGpioTargetValueInt,HIGH);
      }
      else if (actionAdcActionValueStr == "off")
      {
        _debugMessage->debug("Setting " + String(actionAdcGpioTargetValueInt) + " to LOW");
        setDigitalOutput(actionAdcGpioTargetValueInt, LOW);
        _espConfig->setPinGpioDigitalStatusChanged(actionAdcGpioTargetValueInt,1);
        _espConfig->setPinGpioDigitalStatus(actionAdcGpioTargetValueInt,LOW);
      }
      //_debugMessage->debug("Adc index: " + String(cont) + " - actionAdcTimeBeforeActionReversalValueInt: " + String(actionAdcTimeBeforeActionReversalValueInt));
      if (actionAdcTimeBeforeActionReversalValueInt > 0)
      {
        //_debugMessage->debug("Configuring adcActionWhenReverseInMillis[" + String(cont) + "] to " + String((millis() + actionAdcTimeBeforeActionReversalValueInt)));
        pwmAdcDataLocal->adcActionWhenReverseInMillis[cont] = (millis() + actionAdcTimeBeforeActionReversalValueInt);
      }

      pwmAdcDataLocal->adcActionIndexLastTimeInMillis[cont] = millis();
    }
  } 

  // set the previous values arrays
  for (int cont = 0 ; cont < pwmAdcDataLocal->totalGPIO ; cont++)
  {
    pwmAdcDataLocal->pinGpioAdcPreviousValue[cont] = pwmAdcDataLocal->pinGpioAdcValue[cont];
    pwmAdcDataLocal->pinGpioAdcValue[cont] = adcValueGpioAdcArray[cont];
    pwmAdcDataLocal->pinAnalogOnlyPreviousValue[cont] = pwmAdcDataLocal->pinAnalogOnlyValue[cont];
    pwmAdcDataLocal->pinAnalogOnlyValue[cont] = adcValueAnalogOnlyArray[cont];
  }
  
}*/

void GpioManager::checkGpioChange(MqttManagerOut *mqttManagerOut, volatile PwmAdcData *pwmAdcDataLocal)
{
  for (int cont = 0; cont < pwmAdcDataLocal->totalGPIO; cont++)
  {
    checkGpioDigitalStatusChanged(mqttManagerOut, pwmAdcDataLocal, cont);

    checkGpioPwmStatusChanged(mqttManagerOut, pwmAdcDataLocal, cont);

    checkSendAdcGpioValue(mqttManagerOut, pwmAdcDataLocal, cont);

    checkSendAdcAnalogOnlyValue(mqttManagerOut, pwmAdcDataLocal, cont);
  }
}

void GpioManager::checkGpioDigitalStatusChanged(MqttManagerOut *mqttManagerOut, volatile PwmAdcData *pwmAdcDataLocal, int position)
{
  if (pwmAdcDataLocal->pinGpioDigitalStatusChanged[position] == 1)
  {
    int pinGpioDigitalStatusLocal = pwmAdcDataLocal->pinGpioDigitalStatus[position];
    String mqttKey[2];
    String mqttValue[2];
    mqttKey[0] = "gpio";
    mqttValue[0] = String(position);
    mqttKey[1] = "status";
    if (pinGpioDigitalStatusLocal == HIGH)
    {
      mqttValue[1] = "high";
    }
    else
    {
      mqttValue[1] = "low";
    }
    _debugMessage->debug("GPIO Digital Status Changed - GPIO: " + String(position) + " -  New Status: " + mqttValue[1]);
    pwmAdcDataLocal->pinGpioDigitalStatusChanged[position] = 0;
    mqttManagerOut->publishMessageJson(mqttKey, mqttValue, 2, "InfoDigitalGpio");
  }
}
void GpioManager::checkGpioPwmStatusChanged(MqttManagerOut *mqttManagerOut, volatile PwmAdcData *pwmAdcDataLocal, int position)
{
  if (pwmAdcDataLocal->pinGpioPwmStatusChanged[position] == 1)
  {
    int pinGpioPwmStatusLocal = pwmAdcDataLocal->pinGpioPwmStatus[position];
    String mqttKey[2];
    String mqttValue[2];
    mqttKey[0] = "gpio";
    mqttValue[0] = String(position);
    mqttKey[1] = "status";
    mqttValue[1] = String(pinGpioPwmStatusLocal);
    _debugMessage->debug("GPIO PWM Value Changed - GPIO: " + String(position) + " -  New Value: " + mqttValue[1]);
    pwmAdcDataLocal->pinGpioPwmStatusChanged[position] = 0;
    mqttManagerOut->publishMessageJson(mqttKey, mqttValue, 2, "InfoPwmGpio");
  }
}

void GpioManager::checkSendAdcGpioValue(MqttManagerOut *mqttManagerOut, volatile PwmAdcData *pwmAdcDataLocal, int position)
{
  if (pwmAdcDataLocal->sendAdcGpioValue[position] == 1)
    {
      int adcLocal = getAdcValue(String(position));
      String mqttKey[2];
      String mqttValue[2];
      mqttKey[0] = "gpio";
      mqttValue[0] = String(position);
      mqttKey[1] = "value";
      mqttValue[1] = String(adcLocal);
      pwmAdcDataLocal->sendAdcGpioValue[position] = 0;
      mqttManagerOut->publishMessageJson(mqttKey, mqttValue, 2, "InfoAdcGpio");
    }
}

void GpioManager::checkSendAdcAnalogOnlyValue(MqttManagerOut *mqttManagerOut, volatile PwmAdcData *pwmAdcDataLocal, int position)
{
  if (pwmAdcDataLocal->sendAdcAnalogOnlyValue[position] == 1)
    {
      String adcStr = "a" + String(position);
      int adcLocal = getAdcValue(String(position));
      String mqttKey[2];
      String mqttValue[2];
      mqttKey[0] = "gpio";
      mqttValue[0] = adcStr;
      mqttKey[1] = "value";
      mqttValue[1] = String(adcLocal);
      pwmAdcDataLocal->sendAdcAnalogOnlyValue[position] = 0;
      mqttManagerOut->publishMessageJson(mqttKey, mqttValue, 2, "InfoAdcGpio");
    }
}

/*void GpioManager::checkAdcReverse(volatile PwmAdcData *pwmAdcDataLocal)
{
  int adcTotalInt = String(_espConfig->getDataStore()->getValue("action_adc_total")).toInt();
  for (int cont = 0 ; cont < adcTotalInt; cont++)
  {
    if ((pwmAdcDataLocal->adcActionWhenReverseInMillis[cont] > 0) and (millis() > pwmAdcDataLocal->adcActionWhenReverseInMillis[cont]))
    {
      _debugMessage->debug("Reversing ADC Action " + String(cont));
      pwmAdcDataLocal->adcActionWhenReverseInMillis[cont] = 0;

      String actionAdcActionStr = "action_adc_action_r_" + String(cont);
      String actionAdcActionValueStr = String(_espConfig->getDataStore()->getValue(actionAdcActionStr.c_str()));
      String actionAdcGpioTargetStr = "action_adc_gpio_target_r_" + String(cont);
      int actionAdcGpioTargetValueInt = String(_espConfig->getDataStore()->getValue(actionAdcGpioTargetStr.c_str())).toInt();

      if (actionAdcActionValueStr == "reverse")
      {
        _debugMessage->debug("Reversing GPIO " + String(actionAdcGpioTargetValueInt));
        int gpioStatus = getDigitalOutput(actionAdcGpioTargetValueInt);
        setDigitalOutput(actionAdcGpioTargetValueInt, !gpioStatus);
        _espConfig->setPinGpioDigitalStatusChanged(actionAdcGpioTargetValueInt,1);
        _espConfig->setPinGpioDigitalStatus(actionAdcGpioTargetValueInt,!gpioStatus);
      }
      else if (actionAdcActionValueStr == "on")
      {
        _debugMessage->debug("Setting GPIO " + String(actionAdcGpioTargetValueInt) + " to LOW");
        setDigitalOutput(actionAdcGpioTargetValueInt, LOW);
        _espConfig->setPinGpioDigitalStatusChanged(actionAdcGpioTargetValueInt,1);
        _espConfig->setPinGpioDigitalStatus(actionAdcGpioTargetValueInt,LOW);
      }
      else if (actionAdcActionValueStr == "off")
      {
        _debugMessage->debug("Setting GPIO " + String(actionAdcGpioTargetValueInt) + " to HIGH");
        setDigitalOutput(actionAdcGpioTargetValueInt, HIGH);
        _espConfig->setPinGpioDigitalStatusChanged(actionAdcGpioTargetValueInt,1);
        _espConfig->setPinGpioDigitalStatus(actionAdcGpioTargetValueInt,HIGH);
      }

    }
  }
}*/

/*void GpioManager::checkGpioDigitalReverse()
{
  int gpioDigitalTotalInt = String(_espConfig->getDataStore()->getValue("action_digital_total")).toInt();
  for (int cont = 0 ; cont < gpioDigitalTotalInt; cont++)
  {
    if ((_espConfig->getGpioDigitalActionIndexWhenReverseInMillis()[cont] > 0) and (millis() > _espConfig->getGpioDigitalActionIndexWhenReverseInMillis()[cont]))
    {
      _debugMessage->debug("Reversing Gpio Digital Action " + String(cont));
      _espConfig->getGpioDigitalActionIndexWhenReverseInMillis()[cont] = 0;

      String actionGpioDigitalActionStr = "action_digital_action_r_" + String(cont);
      String actionGpioDigitalActionValueStr = String(_espConfig->getDataStore()->getValue(actionGpioDigitalActionStr.c_str()));
      String actionGpioDigitalGpioTargetStr = "action_digital_gpio_target_r_" + String(cont);
      int actionGpioDigitalGpioTargetValueInt = String(_espConfig->getDataStore()->getValue(actionGpioDigitalGpioTargetStr.c_str())).toInt();

      if (actionGpioDigitalActionValueStr == "reverse")
      {
        _debugMessage->debug("Reversing GPIO " + String(actionGpioDigitalGpioTargetValueInt));
        int gpioStatus = getDigitalOutput(actionGpioDigitalGpioTargetValueInt);
        setDigitalOutput(actionGpioDigitalGpioTargetValueInt, !gpioStatus);
        _espConfig->setPinGpioDigitalStatusChanged(actionGpioDigitalGpioTargetValueInt,1);
        _espConfig->setPinGpioDigitalStatus(actionGpioDigitalGpioTargetValueInt,!gpioStatus);
      }
      else if (actionGpioDigitalActionValueStr == "on")
      {
        _debugMessage->debug("Setting GPIO " + String(actionGpioDigitalGpioTargetValueInt) + " to LOW");
        setDigitalOutput(actionGpioDigitalGpioTargetValueInt, LOW);
        _espConfig->setPinGpioDigitalStatusChanged(actionGpioDigitalGpioTargetValueInt,1);
        _espConfig->setPinGpioDigitalStatus(actionGpioDigitalGpioTargetValueInt,LOW);
      }
      else if (actionGpioDigitalActionValueStr == "off")
      {
        _debugMessage->debug("Setting GPIO " + String(actionGpioDigitalGpioTargetValueInt) + " to HIGH");
        setDigitalOutput(actionGpioDigitalGpioTargetValueInt, HIGH);
        _espConfig->setPinGpioDigitalStatusChanged(actionGpioDigitalGpioTargetValueInt,1);
        _espConfig->setPinGpioDigitalStatus(actionGpioDigitalGpioTargetValueInt,HIGH);
      }

    }
  }
}*/

/*void GpioManager::executeDigitalAction(uint32_t gpio, int gpioStatus)
{
  DebugMessage debugMessageLocal = DebugMessage();
  int digitalActionTotalInt = String(_espConfig->getDataStore()->getValue("action_digital_total")).toInt();
  debugMessageLocal.debug("Total of digital actions: " + String(digitalActionTotalInt) );
}*/

uint32_t GpioManager::getDigitalOutput(uint32_t gpio)
{
  return (digitalRead(gpio));
}

void GpioManager::setDigitalOutput(uint32_t gpio, uint32_t status)
{
  digitalWrite(gpio,status);
}
