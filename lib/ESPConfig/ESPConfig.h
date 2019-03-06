/**
 * @file ESPConfig.h
 * @author Fabricio Tamusiunas (fabricio@tamusiunas.com)
 * @brief Object to carry config parameters
 * @version 0.1
 * @date 2019-02-11
 * 
 * @copyright Copyright (c) 2019
 * 
 */

#ifndef ESPConfig_h
#define ESPConfig_h

#include "DataStore.h"

class ESPConfig
{
public:
  /**
   * @brief Construct a new ESPConfig object
   * 
   * @param pinGpioArray pointer to pinGpioArray
   * @param pinGpioAvaliable Pointer to pinGpioAvaliable array
   * @param pinGpioAdcChannelArray Pointer to pinGpioAdcChannelArray array
   * @param pinGpioAdcNumberArray Pointer to pinGpioAdcNumberArray array
   * @param pinGpioInOut Pointer to pinGpioInOut array
   * @param pinGpioDesc Pointer for char pointers with GPIO HW descriptions
   * @param pinPwmValue "Discontinued" - To be removed
   * @param totalGpio Total of GPIO for the board
   * @param pwmChannelGpioHw Pointer to pwmChannelGpioHw array - will be removed
   * @param pwmChannelTotalHw Total of hardware PWM channels for the board - will be removed
   * @param pwmChannelGpioSw Pointer to pwmChannelGpioSw array - will be removed
   * @param pwmChannelTotalSw Total of software PWM channels for the board - will be removed
   * @param dataStore Pointer to DataStore object
   */
  ESPConfig(int *pinGpioArray, int *pinGpioAvaliable, int *pinGpioAdcChannelArray, int *pinGpioAdcNumberArray,
            int *pinGpioInOut, const char **pinGpioDesc, int *pinPwmValue, int totalGpio, volatile int *pwmChannelGpioHw,
            volatile int pwmChannelTotalHw, volatile int *pwmChannelGpioSw, int pwmChannelTotalSw, int *pinGpioMode, volatile int *pinGpioDigitalStatusChanged, 
            volatile int *pinGpioDigitalStatus, volatile int *pinGpioAdcValue, volatile int *pinGpioAdcPreviousValue, 
            volatile bool *pinPwmEnable, volatile int *pinGpioPwmStatusChanged, volatile int *pinGpioPwmStatus, DataStore *dataStore);
  ~ESPConfig();

  /**
   * @brief Get the Pin Gpio Array object
   * 
   * @return int* Pointer to pinGpioArray
   */
  int *getPinGpioArray();
  int *getPinGpioAvaliable();
  int *getPinGpioAdcChannelArray();
  int *getPinGpioAdcNumberArray();
  int *getPinGpioInOut();
  volatile int getPwmChannelTotalHw();
  volatile int *getPwmChannelHw();
  volatile int *getPinGpioDigitalStatusChanged();
  volatile int *getPinGpioDigitalStatus();
  volatile int *getPinGpioAdcValue();
  volatile int *getPinGpioAdcPreviousValue();
  volatile int *getPinGpioPwmStatusChanged();
  volatile int *getPinGpioPwmStatus();
  volatile bool *getPinPwmEnable();
  const char **getPinGpioDesc();
  int *getPinPwmValue();
  int *getPinGpioMode();
  int getTotalGpio();
  DataStore *getDataStore();
  volatile int getPwmChannelHwByGpio(int gpio);
  int addGpioToPwmChanneHw(int gpio);
  int getPwmChannelSwByGpio(int gpio);
  int addGpioToPwmChanneSw(int gpio);
  void setPinGpioMode(int gpio, byte mode);
  void setPinGpioDigitalStatusChanged(int gpio, byte changed);
  void setPinGpioDigitalStatus(int gpio, byte status);
  void setPinGpioAdcValue(int gpio, int AdcValue);
  void setPinGpioAdcPreviousValue(int gpio, int AdcValue);
  void setPinPwmEnable(int gpio, int isPwm);
  void setPinGpioPwmStatusChanged(int gpio, int changed);
  void setPinGpioPwmStatus(int gpio, int pwmStatus);

private:
  int *_pinGpioArray, *_pinGpioAvaliable, *_pinGpioAdcChannelArray, *_pinGpioAdcNumberArray, *_pinGpioInOut,
      *_pinPwmValue, _totalGpio, _pwmChannelTotalSw, *_pinGpioMode;
  volatile int *_pinGpioDigitalStatusChanged, *_pinGpioDigitalStatus, *_pinGpioAdcValue, *_pwmChannelGpioSw, 
               *_pinGpioAdcPreviousValue, *_pwmChannelGpioHw, _pwmChannelTotalHw, *_pinGpioPwmStatusChanged, 
               *_pinGpioPwmStatus;
  volatile bool *_pinPwmEnable;
  const char **_pinGpioDesc;
  DataStore *_dataStore;
};

#endif
