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

void GpioManager::checkAdcGpioActions(MqttManagerOut *mqttManagerOut, volatile PwmAdcData *pwmAdcDataLocal)
{
  int adcValueGpipAdcArray[pwmAdcDataLocal->totalGPIO];
  int adcValueAnalogOnlyArray[pwmAdcDataLocal->totalGPIO];
  for (int cont = 0 ; cont < pwmAdcDataLocal->totalGPIO ; cont++)
  {
    adcValueGpipAdcArray[cont] = -1;
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
        int adcValue = 0;
        for (int adcCont = 0 ; adcCont < 16 ; adcCont++)
        {
          adcValue += getAdcValue(adcGpioOriginValueStr);
        }
        adcValue = adcValue/16;
        adcValueAnalogOnlyArray[adcAnalogOnlyOriginInt] = adcValue;
        //_debugMessage->debug("adcValueGpipAnalogOnlyArray[" + String(adcAnalogOnlyOriginInt) + "] = " + String(adcValue));
      }
    }
    else
    {
      int adcGpioOriginInt = String(_espConfig->getDataStore()->getValue(adcGpioOriginStr.c_str())).toInt();
      if (adcValueGpipAdcArray[adcGpioOriginInt] == -1 )
      {
        int adcValue = 0;
        for (int adcCont = 0 ; adcCont < 16 ; adcCont++)
        {
          adcValue += getAdcValue(adcGpioOriginValueStr);
        }
        adcValue = adcValue/16;
        adcValueGpipAdcArray[adcGpioOriginInt] = adcValue;
        //_debugMessage->debug("adcValueGpipAdcArray[" + String(adcGpioOriginInt) + "] = " + String(adcValue));
      }
    }
  }

  bool runAction = false;
  for (int cont = 0 ; cont < actionAdcTotal ; cont++)
  {
    bool analogOnly = false;
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

    if (analogOnly)
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
    else
    {
      int adcValue = adcValueGpipAdcArray[adcGpioOriginInt];
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

  // set the previous values arrays
  for (int cont = 0 ; cont < pwmAdcDataLocal->totalGPIO ; cont++)
  {
    pwmAdcDataLocal->pinGpioAdcPreviousValue[cont] = pwmAdcDataLocal->pinGpioAdcValue[cont];
    pwmAdcDataLocal->pinGpioAdcValue[cont] = adcValueGpipAdcArray[cont];
    pwmAdcDataLocal->pinAnalogOnlyPreviousValue[cont] = pwmAdcDataLocal->pinAnalogOnlyValue[cont];
    pwmAdcDataLocal->pinAnalogOnlyValue[cont] = adcValueAnalogOnlyArray[cont];
  }
  if (runAction)
  {
    //run the actions
  }

  
}

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

void GpioManager::executeDigitalAction(uint32_t gpio, int gpioStatus)
{
  DebugMessage debugMessageLocal = DebugMessage();
  int digitalActionTotalInt = String(_espConfig->getDataStore()->getValue("action_digital_total")).toInt();
  debugMessageLocal.debug("Total of digital actions: " + String(digitalActionTotalInt) );
}

uint32_t GpioManager::getDigitalOutput(uint32_t gpio)
{
  return (digitalRead(gpio));
}

void GpioManager::setDigitalOutput(uint32_t gpio, uint32_t status)
{
  digitalWrite(gpio,status);
  String mqttKey[1];
  String mqttValue[1];
  mqttKey[0] = "gpio";
  if (status == HIGH)
  {
    mqttValue[0] = "high";
  }
  else
  {
    mqttValue[0] = "low";
  }
}