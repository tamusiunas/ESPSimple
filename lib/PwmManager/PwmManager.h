/**
 * @file PwmManager.h
 * @author Fabricio Tamusiunas (fabricio@tamusiunas.com)
 * @brief Pwm Manager
 * @version 0.1
 * @date 2019-02-11
 * 
 * @copyright Copyright (c) 2019
 * 
 */

#ifndef PwmManager_h
#define PwmManager_h

#include "ESPConfig.h"
#include "GpioManager.h"

class PwmManager
{
public:
    /**
     * @brief Construct a new Pwm Manager object
     * 
     * @param dataStore DataStore
     */
    PwmManager (ESPConfig *espConfig);

    /**
     * @brief Destroy the Pwm Manager object
     * 
     */
    ~PwmManager ();

    /**
     * @brief Set Pwm Value
     * 
     * @param gpio GPIO number
     * @param pwmValue Pwm value
     * @return true Sucess
     * @return false Failure
     */
    bool setPwm(int gpio, int pwmValue);

    /**
     * @brief Get Pwm Value
     * 
     * @param gpio GPIO number
     * @return int Pwm value
     */
    int getPwm(int gpio);
};

#endif