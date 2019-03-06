#include "ESPConfig.h"
#include "GpioManager.h"

struct AlexaStruct
{
    ESPConfig *espConfig;
    GpioManager *gpioManager;
    //volatile PwmAdcData *pwmAdcDataLocal;
};