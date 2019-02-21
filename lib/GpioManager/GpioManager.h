/**
 * @file GpioManager.h
 * @author Fabricio Tamusiunas (fabricio@tamusiunas.com)
 * @brief Read and configure GPIO (mode and PWM)
 * @version 0.1
 * @date 2019-02-09
 * 
 * @copyright Copyright (c) 2019
 * 
 */

#ifndef GpioManager_h
#define GpioManager_h

#include "Arduino.h"
#include "ESPConfig.h"
#include "DataStore.h"
#include "GpioActionInterrupts.h"
#include "GpioCommons.h"
#ifdef ESP8266
// PubSubClient.h, WiFiAliases.h and JsonManager.h must be removed in a near future. They are here "probably" for a PATH issue in Platformio with ESP8266
#include <PubSubClient.h>
#include "WiFiAliases.h"
#include "JsonManager.h"
#include <FunctionalInterrupt.h>
#include <Ticker.h>
#endif
#ifdef ESP32
#include <driver/gpio.h>
#include <driver/ledc.h>
#include <soc/ledc_struct.h>
#define DUTY_BIT_DEPTH LEDC_TIMER_10_BIT
#define ZEROCROSSTIMER 0
#define NONZEROCROSSTIMER 1
#endif
#include <MqttManagerOut.h>
#define PWMSTEPS 1024


class GpioManager
{
public:
  /**
   * @brief Construct a new Gpio Manager object
   * 
   * @param espConfig ESPConfig Object with config parameters
   */
  //GpioManager(ESPConfig *espConfig, MqttManagerOut *mqttManagerOut);
  

  GpioManager(ESPConfig *espConfig);
  
  /**
   * @brief Destroy the Gpio Manager object
   * 
   */
  ~GpioManager();
  
  /**
   * @brief Initialize GPIO config from ESPConfig Parameters
   * 
   */
  void initializeGpio();
  
  /**
   * @brief Set the Pwm frequency
   * 
   * @param gpio GPIO
   * @param pwm frequency (MAX and MIN defined in ESPConfig)
   */
  void setPwm(int gpio, int pwm);

  /**
   * @brief Get Gpio output
   * 
   * @return uint32_t GPIO Status (LOW or HIGH)
   */
  uint32_t getDigitalOutput(uint32_t gpio);

  /**
   * @brief Set Gpio output
   * 
   * @param gpio GPIO
   * @param status GPIO Status (LOW or HIGH)
   * @return true Sucess
   * @return false Failure
   */
  bool setDigitalOutput(uint32_t gpio, uint32_t status);

private:
  /**
   * @brief Configure the GPIO as an interrupt (EDGE in)
   * 
   * @param gpio GPIO
   */
  void setInterrupt(uint32_t gpio);
  
  /**
   * @brief Set GPIO Mode
   * 
   * @param gpio GPIO
   * @param mode Mode (INPUT, OUTPUT or INPUT_PULLUP)
   */
  void setGpioMode(int gpio, byte mode);
  
  /**
   * @brief Configure the GPIO as a PWM with zero cross (usefull for TRIAC)
   * 
   * @param gpio GPIO
   */
  void addGpioPwmZeroCross(int gpio);
  
  /**
   * @brief Configure the GPIO as a PWM without zero cross (usefull for DC)
   * 
   * @param gpio GPIO
   */
  void addGpioPwmNonZeroCross(int gpio);
  
  #ifdef ESP32
  /**
   * @brief Initialize ESP32 timers
   * 
   */
  void initializeTimers();

  /**
   * @brief Configure Pwm for a specific GPIO for ESP32
   *  
   * @param gpio    GPIO (gpio_num_t)
   * @param channel Channel (ledc_channel_t)
   * @param timer   Timer (ledc_timer_t) ZEROCROSSTIMER (0) or NONZEROCROSSTIMER (1)
   */
  void configGpioPwm(uint32_t gpio, int channel, int timer);
  
  /**
   * @brief Handle interrupt (excepts to zero cross interrupt) for ESP32
   * 
   * @param arg ARG
   */
  static void IRAM_ATTR handleInterrupt(void* arg);

  /**
   * @brief Handle zero cross interrupt for ESP32
   * 
   * @param arg ARG
   */
  static void IRAM_ATTR handleZeroCrossInterrupt(void* arg);

  /**
   * @brief Configure Timer for Pwm Channel for ESP32
   * 
   * @param timer     Timer Num (2-bit)
   * @param frequency Frequency in hz
   */
  void configGpioTimer(int timer, int frequency);

  #elif ESP8266

  /**
   * @brief Initialize ESP8266 timers
   * 
   */
  void initializeTimers();

  /**
   * @brief Configure Pwm for a specific GPIO for ESP8266
   * 
   * @param gpio 
   */
  void configGpioPwm(uint32_t gpioInterruptPin);

  /**
   * @brief Handle Timer Interruption for ESP8266
   * 
   */
  void ICACHE_RAM_ATTR handleTimer();

  /**
   * @brief Handle interrupt (excepts to zero cross interrupt) for ESP8266
   * 
   * @param gpio GPIO that generated the interrupt 
   */
  void ICACHE_RAM_ATTR handleInterrupt(void *arg);

  /**
   * @brief Handle zero cross interrupt for ESP8266
   * 
   */
  void ICACHE_RAM_ATTR handleZeroCrossInterrupt();
  Ticker _tickerPwm;
  #endif

  /**
   * @brief Get ESPConfig
   * 
   * @return ESPConfig* Pointer to ESPConfig
   */
  ESPConfig *getESPConfig();

  /**
   * @brief Execute digital action for GPIO
   * 
   * @param gpio GPIO
   * @param gpioStatus GPIO Status
   * @return true Sucess
   * @return false False
   */
  void executeDigitalAction(uint32_t gpio, int gpioStatus);

  ESPConfig *_espConfig;
  // MqttManagerOut *_mqttManagerOut;
  bool interrupInitialized = false;
  bool _zeroCrossEnable = false;

};

#endif
