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
            int *pinGpioInOut, const char **pinGpioDesc, int *pinPwmValue, int totalGpio, int *pwmChannelGpioHw,
            int pwmChannelTotalHw, int *pwmChannelGpioSw, int pwmChannelTotalSw, int *pinGpioMode, volatile int *pinGpioStatusChanged, 
            volatile int *pinGpioDigitalStatus, DataStore *dataStore);
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
  volatile int *getPinGpioStatusChanged();
  volatile int *getPinGpioDigitalStatus();
  const char **getPinGpioDesc();
  int *getPinPwmValue();
  int *getPinGpioMode();
  int getTotalGpio();
  DataStore *getDataStore();
  int getPwmChannelHwByGpio(int gpio);
  int addGpioToPwmChanneHw(int gpio);
  int getPwmChannelSwByGpio(int gpio);
  int addGpioToPwmChanneSw(int gpio);
  void setPinGpioMode(int gpio, byte mode);
  void setPinGpioStatusChanged(int gpio, byte changed);
  void setPinGpioDigitalStatus(int gpio, byte status);
private:
  int *_pinGpioArray, *_pinGpioAvaliable, *_pinGpioAdcChannelArray, *_pinGpioAdcNumberArray, *_pinGpioInOut,
      *_pinPwmValue, _totalGpio, *_pwmChannelGpioHw, *_pwmChannelGpioSw, _pwmChannelTotalHw, _pwmChannelTotalSw, 
      *_pinGpioMode;
  volatile int *_pinGpioStatusChanged, *_pinGpioDigitalStatus;
  const char **_pinGpioDesc;
  DataStore *_dataStore;
};

#endif
