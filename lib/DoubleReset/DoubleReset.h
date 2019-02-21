/**
 * @file DoubleReset.h
 * @author Fabricio Tamusiunas (fabricio@tamusiunas.com)
 * @brief Check if reset button was pressed twice to enter in web config mode
 * @version 0.1
 * @date 2019-02-11
 * 
 * @copyright Copyright (c) 2019
 * 
 */
#ifndef DoubleReset_h
#define DoubleReset_h

#include <Arduino.h>
#include "EEPROM.h"
#include "time.h"
#include "sys/time.h"

class DoubleReset
{
public:
  /**
   * @brief Construct a new Double Reset object
   * 
   * @param timeout Timeout for second push
   */
  DoubleReset(unsigned int timeout);

  /**
   * @brief Construct a new Double Reset object
   * 
   * Timeout is five seconds
   * 
   */
  DoubleReset();

  /**
   * @brief Destroy the Double Reset object
   * 
   */
  ~DoubleReset();

  /**
   * @brief Start counter
   * 
   */
  void start();

  /**
   * @brief Stop counter
   * 
   */
  void stop();

  /**
   * @brief Check for time
   * 
   */
  void handle();

  /**
   * @brief Get the how many resets were pressed before timeout
   * 
   * @return int 
   */
  int getResetValue();

  /**
   * @brief Set the Timeout object
   * 
   * @param timeout Timout for double reset
   */
  void setTimeout(unsigned int timeout);

private:
  unsigned int  _timeout;
  int _resetValue;
  unsigned int _timeInMillis;
  bool _isTimeOut = false;
  bool _started = false;
};

#endif
