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

/*GpioManager::GpioManager(ESPConfig *espConfig, MqttManagerOut *mqttManagerOut)
{
  _espConfig = espConfig;
  _mqttManagerOut = mqttManagerOut;
  initializeTimers();
}*/

GpioManager::GpioManager(ESPConfig *espConfig)
{
  _espConfig = espConfig;
  //_mqttManagerOut = NULL;
  initializeTimers();
}

GpioManager::~GpioManager()
{

}

void GpioManager::initializeGpio()
{
  int totalParameters = _espConfig->getDataStore()->getDataParametersCount();
  Serial.println("totalParameters: " + String (totalParameters));
  for (int cont=0 ; cont < totalParameters ; ++cont )
  {
    DataParameter *dataParameter = _espConfig->getDataStore()->getParameterByPos(cont);
    if (dataParameter != NULL)
    {
      if (String(dataParameter->getField()).indexOf("gpio_mode_") == 0)
      {
        int gpioInt = String(String(dataParameter->getField()).substring(10)).toInt();
        const char *gpioModeCChar = dataParameter->getValue();
        Serial.println("Found Gpio Mode: " + String(gpioInt) + "(" + String(gpioModeCChar) + ")");
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
        Serial.println("Found PWM: " + String(PwmGpio) + "-" + gpioSource);
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
  Serial.println("setGpioMode: " + String(gpio) + "(" + String(mode) + ")");
}

ESPConfig *GpioManager::getESPConfig()
{
  return _espConfig;
}

void GpioManager::checkAdcGpioActions(MqttManagerOut *mqttManagerOut, volatile PwmAdcData *pwmAdcDataLocal)
{
  int actionAdcTotal = String(_espConfig->getDataStore()->getValue("action_adc_total")).toInt();
  for (int cont = 0 ; cont < actionAdcTotal ; cont++)
  {
    
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
      Serial.println("Gpio status changed: " + String(position) + ". New status: " + String(pinGpioDigitalStatusLocal));
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
      pwmAdcDataLocal->pinGpioDigitalStatusChanged[position] = 0;
      mqttManagerOut->publishMessageJson(mqttKey, mqttValue, 2, "InfoDigitalGpio");
    }
}
void GpioManager::checkGpioPwmStatusChanged(MqttManagerOut *mqttManagerOut, volatile PwmAdcData *pwmAdcDataLocal, int position)
{
  if (pwmAdcDataLocal->pinGpioPwmStatusChanged[position] == 1)
    {
      int pinGpioPwmStatusLocal = pwmAdcDataLocal->pinGpioPwmStatus[position];
      Serial.println("Gpio Pwm status changed: " + String(position) + ". New status: " + String(pinGpioPwmStatusLocal));
      String mqttKey[2];
      String mqttValue[2];
      mqttKey[0] = "gpio";
      mqttValue[0] = String(position);
      mqttKey[1] = "status";
      mqttValue[1] = String(pinGpioPwmStatusLocal);
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
  int digitalActionTotalInt = String(_espConfig->getDataStore()->getValue("action_digital_total")).toInt();
  Serial.print("Total of digital actions: ");
  Serial.println(digitalActionTotalInt);
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