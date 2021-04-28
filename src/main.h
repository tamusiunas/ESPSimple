#include "DataStore.h"
#include "SpiffsManager.h"
#include "WiFiSTIManager.h"
#include "WebConfig.h"
#include "ESPConfig.h"
#include "DoubleReset.h"
#include "OTAHandler.h"
#include "SyslogManager.h"
#include "ServersManagement.h"
#include "GpioManager.h"
#include "MqttManagerIn.h"
#include "MqttManagerOut.h"
#include "AmazonAlexa.h"
#include "DebugMessage.h"
#include "DhtManager.h"
#include "CheckActionMqtt.h"
//#include <RH_ASK.h>

#define FORMAT_SPIFFS_IF_FAILED true

#ifdef ESP8266
#define TOTALGPIO 17 // Total of GPIO for ESP32
#define TOTALPWMSW 8
#define TOTALPWMHW 8
volatile int pwmChannelGpioHw[] =                      {-1,-1,-1,-1,-1,-1,-1}; // each position is a channel (0-8)
volatile int pwmChannelGpioSw[] =                      {-1,-1,-1,-1,-1,-1,-1}; // each position is a channel (0-8)
int pinGpioArray[] =                                   { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9,10,11,12,13,14,15,16};
int pinGpioAvaliable[] =                               { 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 1, 0, 1, 1, 1, 1, 1};
int pinGpioMode[] =                                    {-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1};
int pinAnalogOnly[] =                                  { 1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1};
volatile int sendAdcGpioValue[] =                      { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}; // send Adc Gpio value to Mqtt/Debug
volatile int sendAdcAnalogOnlyValue[] =                { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}; // send Adc Analog Only to Mqtt/Debug
volatile int pinGpioDigitalStatusChanged[] =           { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}; // Gpio Digital last status changed (false/true)
volatile int pinGpioDigitalStatus[] =                  {-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1}; // Gpio Digital last status (low/high)
volatile int pinGpioPwmStatusChanged[] =               { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}; // Gpio Pwm last value changed (false/true)
volatile int pinGpioPwmStatus[] =                      {-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1}; // Gpio Pwm last value

volatile int pinGpioAdcValue[] =                       {-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1}; // Gpio Adc last value 
volatile int pinGpioAdcPreviousValue[] =               {-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1}; // Gpio Adc previous value 
volatile int pinGpioAdcLastAction[] =                  {-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1}; // Gpio Adc last Action (low/high)

volatile int pinAnalogOnlyValue[] =                    {-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1}; // Analog Only last value
volatile int pinAnalogOnlyPreviousValue[] =            {-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1}; // Analog Only previous value
volatile int pinAnalogOnlyLastAction[] =               {-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1}; // Analog Only last Action (low/high) 

volatile bool pinPwmEnable[] =                         { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}; // Pin PWM enable (false/true)
int pinGpioAdcChannelArray[] =                         {-1 ,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1};
int pinGpioAdcNumberArray[] =                          {-1 ,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1};
int pinGpioInOut[] =                                   { 2, 2, 2, 2, 2, 2, 0, 0, 0, 0, 2, 0, 2, 2, 2, 2, 2};
const char *pinGpioDesc[] =                            {"D3","RXD0","D4","TXD0","D2","D1","","","","","SD3","","D6","D7","D5","D8","D0"};
int pinPwmValue[] =                                    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
#elif defined(ESP32)
#define TOTALGPIO 40 // Total of GPIO for ESP32
#define TOTALPWMSW 8 
#define TOTALPWMHW 8
volatile int pwmChannelGpioHw[] =                      {-1,-1,-1,-1,-1,-1,-1}; // each position is a channel (0-8)
int pwmChannelGpioSw[] =                               {-1,-1,-1,-1,-1,-1,-1}; // each position is a channel (0-8)
int pinGpioArray[] =                                   { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,
                                                        28,29,30,31,32,33,34,35,36,37,38,39};
int pinGpioAvaliable[] =                               { 0, 0, 1, 0, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1,
                                                         0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1};
int pinGpioMode[] =                                    {-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
                                                        -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1};
int pinAnalogOnly[] =                                  {-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
                                                        -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1};
volatile int sendAdcGpioValue[] =                      { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                                                         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
volatile int sendAdcAnalogOnlyValue[] =                { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                                                         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
volatile int pinGpioDigitalStatusChanged[] =           { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                                                         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
volatile int pinGpioDigitalStatus[] =                  {-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
                                                        -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1};
volatile int pinGpioPwmStatusChanged[] =               { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                                                         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
volatile int pinGpioPwmStatus[] =                      {-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
                                                        -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1};
volatile int pinGpioAdcValue[] =                       { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                                                         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
volatile int pinGpioAdcPreviousValue[] =               {-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
                                                        -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1};
volatile int pinGpioAdcLastAction[] =                  {-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
                                                        -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1};
volatile int pinAnalogOnlyValue[] =                    {-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
                                                        -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1};
volatile int pinAnalogOnlyPreviousValue[] =            {-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
                                                        -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1};
volatile int pinAnalogOnlyLastAction[] =               {-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
                                                        -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1};
volatile bool pinPwmEnable[] =                         { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                                                         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
int pinGpioAdcChannelArray[] =                         {-1,-1, 2,-1, 2,-1,-1,-1,-1,-1,-1,-1,-1, 2, 2, 2,-1,-1,-1,-1,-1,-1,-1,-1,-1, 2, 2, 2,
                                                        -1,-1,-1,-1, 1, 1, 1, 1, 1,-1,-1, 1};
int pinGpioAdcNumberArray[] =                          {-1,-1, 2,-1, 0,-1,-1,-1,-1,-1,-1,-1,-1, 4, 6, 3,-1,-1,-1,-1,-1,-1,-1,-1,-1, 8, 9, 7,
                                                        -1,-1,-1,-1, 4, 5, 6, 7, 0,-1,-1, 3};
int pinGpioInOut[] =                                   {-1,-1, 1,-1, 2, 2,-1,-1,-1,-1,-1,-1, 2, 2, 2, 2, 2, 2, 2, 2,-1, 2, 2, 2,-1, 2, 2, 2,
                                                        -1,-1,-1,-1, 2, 2, 0, 0, 0, 0, 0, 0};
const char *pinGpioDesc[] =                            {"D0","D1 (TXD0)","D2","D3 (RXD0)","D4","D5","D6","D7","D8","D9","D10","D11","D12",
                                                        "D13","D14","D15","D16 (RX2)","D17 (DX2)","D18","D19","D20","D21","D22","D23","D24",
                                                        "D25","D26","D27","D28","D29","D30","D31","D32","D33","D34","D35","D36 (VP)","D37",
                                                        "D38","D39 (VN)"};
int pinPwmValue[] =                                    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                                                         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
#endif

unsigned long lastTimeinMillisDoubleReset = 0;
unsigned long lastTimeinMillisOta = 0;
unsigned long lastTimeinMillisMqtt = 0;
unsigned long lastTimeinMillisNtp = 0;
unsigned long lastTimeinMillisAdc = 0;
unsigned long lastTimeinMillisDht = 0;

volatile PwmAdcData *pwmAdcDataLocal;
DoubleReset doubleReset = DoubleReset(5000);
bool mustStartWebConfig = false;
OTAHandler *otaHandler;
SyslogManager *syslogManager;
ESPConfig *espConfig; // Config values passed through objects
WiFiSTIManager *wiFiSTIManager; // To configure WiFi parameters
MqttManagerIn *mqttManagerIn; // Receive MQTT Messages
MqttManagerOut *mqttManagerOut = NULL; // Send MQTT Messages
GpioManager *gpioManager; // Config and read data from GPIO
AmazonAlexa *amazonAlexa; // Send and receive Amazon Alexa Messages
AlexaStruct *alexaStruct; // Struct to carry Alexa useful data
bool isAlexaEnable = false;
WiFiUDP ntpUDP;
DebugMessage *debugMessage;
DhtManager **dhtManagerArray;
CheckActionMqtt *checkActionMqtt;

//RH_ASK rf_driver();