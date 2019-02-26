/**
 * @file GpioPwmESP32.cpp
 * @author Fabricio Tamusiunas (fabricio@tamusiunas.com)
 * @brief Read and configure GPIO (methods for ESP32)
 * @version 0.1
 * @date 2019-02-09
 * 
 * @copyright Copyright (c) 2019
 * 
 */
#ifdef ESP32

#include "GpioManager.h"

void GpioManager::configGpioPwm(uint32_t gpio, int channel, int timer)
{
  Serial.println("\nconfigGpioPwm");
  ledc_channel_config_t led_config;
  led_config.gpio_num = (gpio_num_t) gpio; // GPIO
  led_config.speed_mode = LEDC_HIGH_SPEED_MODE; // LEDC high speed
  led_config.channel = (ledc_channel_t) channel; // channel from 0 to 7
  led_config.timer_sel = (ledc_timer_t) timer; // timer (2-bit)
  led_config.duty = (1 << DUTY_BIT_DEPTH) - 1; // 0-1023 (config as 1023 since DUTY_BIT_DEPTH = 10-bit)
  led_config.intr_type = LEDC_INTR_DISABLE; // disable LECD interrupt
  Serial.println("led_config.gpio_num: " + String(led_config.gpio_num));
  Serial.println("led_config.speed_mode: " + String(led_config.speed_mode));
  Serial.println("led_config.channel: " + String(led_config.channel));
  Serial.println("led_config.timer_sel: " + String(led_config.timer_sel));
  Serial.println("led_config.duty: " + String(led_config.duty));
  Serial.println("led_config.intr_type: " + String(led_config.intr_type));
  ledc_channel_config(&led_config);
}

void GpioManager::configGpioTimer(int timer, int frequency)
{
  Serial.println("\nconfigGpioTimer");
  ledc_timer_config_t timer_config;
  timer_config.speed_mode = LEDC_HIGH_SPEED_MODE; // LEDC high speed
  timer_config.duty_resolution = DUTY_BIT_DEPTH; // 0-1023 since DUTY_BIT_DEPTH = 10-bit (can be up to 20-bit)
  timer_config.timer_num = (ledc_timer_t) timer; // timer num (2-bit from 0 to 3)
  timer_config.freq_hz = frequency; // frequency (hz)
  Serial.println("timer_config.speed_mode: " + String(timer_config.speed_mode));
  Serial.println("timer_config.duty_resolution: " + String(timer_config.duty_resolution));
  Serial.println("timer_config.timer_num: " + String(timer_config.timer_num));
  Serial.println("timer_config.freq_hz: " + String(timer_config.freq_hz));
  ledc_timer_config(&timer_config);
}

void IRAM_ATTR GpioManager::handleZeroCrossInterrupt(void* arg)
{
  ledc_timer_rst(LEDC_HIGH_SPEED_MODE, (ledc_timer_t) ZEROCROSSTIMER);
}

void GpioManager::initializeTimers()
{
  ledc_fade_func_install(ESP_INTR_FLAG_LOWMED);
  int frequency = 60;
  int zeroCrossGpio = -1;
  if (String(_espConfig->getDataStore()->getValue("pwm_enable_zero_cross")) == "yes")
  {
    _zeroCrossEnable = true;
    String zeroCrossFrequencyStr = _espConfig->getDataStore()->getValue("pwm_zero_cross_frequency");
    if (zeroCrossFrequencyStr != "auto")
    {
      frequency = zeroCrossFrequencyStr.toInt();
    }
    else
    {
      // To-Do Configure Zero Cross Auto
    }
  }
  if (_zeroCrossEnable)
  {
    zeroCrossGpio = String(_espConfig->getDataStore()->getValue("pwm_zero_cross_gpio")).toInt();
    configGpioTimer(ZEROCROSSTIMER,frequency*2); // // channel zero has electric frequency * 2 (for zero cross)
    if (!interrupInitialized)
    {
      gpio_install_isr_service(ESP_INTR_FLAG_LOWMED);
      interrupInitialized = true;
    }
    gpio_set_intr_type((gpio_num_t) zeroCrossGpio, GPIO_INTR_POSEDGE);
    gpio_intr_enable((gpio_num_t) zeroCrossGpio);
    gpio_isr_handler_add((gpio_num_t) zeroCrossGpio, handleZeroCrossInterrupt, NULL);
    //To-Do configure interruption for zero cross
  }
  configGpioTimer(NONZEROCROSSTIMER,5120); // channel zero has frequency of 5 kbit
}

void GpioManager::setPwm(int gpio, int pwm, volatile PwmAdcData *pwmAdcDataLocal)
{
  Serial.println("");
  Serial.println("Pwm: " + String(pwm));
  Serial.println("P1");
  //int channel = _espConfig->getPwmChannelHwByGpio(gpio);
  int channel = -1;

  for (int cont=0 ; cont < pwmAdcDataLocal->totalPwmHw ; cont++)
  {
    Serial.println("P2");
    if (pwmAdcDataLocal->pwmChannelGpioHw[cont] == gpio)
    {
      Serial.println("P3");
      channel = cont;
    }
  }

  Serial.println("channel: " + String(channel));
  int totalSteps = PWMSTEPS;
  Serial.println("totalSteps: " + String(totalSteps));
  int dutyBitDec = 1 << DUTY_BIT_DEPTH;
  Serial.println("dutyBitDec: " + String(dutyBitDec));
  int hpoint = (dutyBitDec - (pwm*(dutyBitDec/totalSteps)));
  Serial.println("hpoint: " + String(hpoint));
  int duty = (pwm*(dutyBitDec/totalSteps));
  Serial.println("duty: " + String(duty));
  duty -= (dutyBitDec - duty - hpoint);
  Serial.println("duty: " + String(duty));
  Serial.println("");
  ledc_set_duty_and_update(LEDC_HIGH_SPEED_MODE, (ledc_channel_t) channel, duty, hpoint);
}

void GpioManager::addGpioPwmZeroCross(int gpio)
{
  int pwmChannel = _espConfig->addGpioToPwmChanneHw(gpio);
  Serial.println("Configuring ZeroCross PWM: " + String(gpio) + " - channel " + String(pwmChannel));
  configGpioPwm(gpio, pwmChannel, ZEROCROSSTIMER);
}

void GpioManager::addGpioPwmNonZeroCross(int gpio)
{
  int pwmChannel = _espConfig->addGpioToPwmChanneHw(gpio);
  Serial.println("Configuring NonZeroCross PWM: " + String(gpio) + " - channel " + String(pwmChannel));
  configGpioPwm(gpio, pwmChannel, NONZEROCROSSTIMER);
}

#endif
