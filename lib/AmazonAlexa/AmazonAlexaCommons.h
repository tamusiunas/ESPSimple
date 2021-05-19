/**
 * @file AmazonAlexaCommons.h
 * @author Fabricio Tamusiunas (fabricio@tamusiunas.com)
 * @brief Struct used by AmazonAlexa
 * @version 0.1
 * @date 2019-02-11
 *  
 * @copyright Copyright (c) 2019
 * 
 */

#include "ESPConfig.h"
#include "GpioManager.h"

struct AlexaStruct
{
    ESPConfig *espConfig;
    GpioManager *gpioManager;
};