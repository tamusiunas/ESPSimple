#include "main.h"

void setup(){
  Serial.begin(115200); // Set serial baud to 115200 bps
  Serial.setDebugOutput(true); // Enable debug output
  doubleReset.start(); // start check for double reset for WEB Config
  // Check if it's a double reset
  if (doubleReset.getResetValue() >= 1)
  {
    mustStartWebConfig = true;
    doubleReset.stop(); // it's a double reset so stop double reset check
    Serial.println("doubleReset.getResetValue(): " + String(doubleReset.getResetValue()));
  }

  DataStore *dataStore = new DataStore(); // initialize DataStore
  SpiffsManager *spiffsManager = new SpiffsManager("/config.json",true,dataStore);
  spiffsManager->readFile();
  dataStore->printParameters(); // print Parameters read from spiffsManager
  // Initialize espConfig with predefined parameters
  espConfig = new ESPConfig(pinGpioArray, pinGpioAvaliable, pinGpioAdcChannelArray, pinGpioAdcNumberArray,
            pinGpioInOut, pinGpioDesc, pinPwmValue, TOTALGPIO, pwmChannelGpioHw, TOTALPWMHW, pwmChannelGpioSw,
            TOTALPWMSW, pinGpioMode, pinGpioDigitalStatusChanged, pinGpioDigitalStatus, pinGpioAdcValue, 
            pinGpioAdcPreviousValue, pinPwmEnable, pinGpioPwmStatusChanged, pinGpioPwmStatus, pinAnalogOnly, dataStore);

  WebConfig webConfig(espConfig,spiffsManager);
  if (mustStartWebConfig)
  {
    Serial.println("Web Config Started");
    webConfig.startSsid(80,"192.168.4.1",NULL,NULL);
    webConfig.processClient();
  }

  Serial.println("Connecting to network");

  wiFiSTIManager = new WiFiSTIManager(espConfig);
  wiFiSTIManager->start();

  otaHandler.setEspConfig(espConfig);
  otaHandler.start();

  syslogManager = new SyslogManager("192.168.130.101", 514);
  syslogManager->sendMessage("main","Teste syslog");

  pwmAdcDataLocal = (PwmAdcData *)malloc(sizeof(PwmAdcData));
  pwmAdcDataLocal->pinGpioAdcPreviousValue = pinGpioAdcPreviousValue;
  pwmAdcDataLocal->pinGpioAdcValue = pinGpioAdcValue;
  pwmAdcDataLocal->pinGpioDigitalStatus = pinGpioDigitalStatus;
  pwmAdcDataLocal->pinGpioDigitalStatusChanged = pinGpioDigitalStatusChanged;
  pwmAdcDataLocal->pinGpioPwmStatusChanged = pinGpioPwmStatusChanged;
  pwmAdcDataLocal->pinGpioPwmStatus = pinGpioPwmStatus;
  pwmAdcDataLocal->pinPwmEnable = pinPwmEnable;
  pwmAdcDataLocal->pwmChannelGpioHw = pwmChannelGpioHw;
  pwmAdcDataLocal->pwmChannelGpioSw = pwmChannelGpioSw;
  pwmAdcDataLocal->totalGPIO = TOTALGPIO;
  pwmAdcDataLocal->totalPwmHw = TOTALPWMHW;
  pwmAdcDataLocal->totalPwmSw = TOTALPWMSW;

  Serial.println("Configuring MQTT");
  String mqttServerStr = dataStore->getValue("mqtt_ip_address");
  if (mqttServerStr != "")
  {
    int mqttPort = 0;
    if (String(dataStore->getValue("mqtt_port")) != "")
    {
      mqttPort = String(dataStore->getValue("mqtt_port")).toInt();
    }
    if (mqttPort <= 0)
    {
      mqttPort = 1883;
    }
    Serial.println("mqttServerStr: " + mqttServerStr);
    Serial.println("mqttPort: " + String(mqttPort));
    mqttManagerIn = new MqttManagerIn(mqttServerStr.c_str(), mqttPort, espConfig, gpioManager, pwmAdcDataLocal);
    mqttManagerIn->connect();
    mqttManagerOut = new MqttManagerOut(mqttServerStr.c_str(), mqttPort);
    mqttManagerOut->connect();
  }

  Serial.println("Configuring GPIO");
  gpioManager = new GpioManager(espConfig);
  //gpioManager = new GpioManager(espConfig);
  gpioManager->initializeGpio();

  if (strcmp(dataStore->getValue("alexa_enable"),"yes") == 0)
  {
    Serial.println("Configuring Alexa");
    alexaStruct = (AlexaStruct *)malloc(sizeof(AlexaStruct));
    alexaStruct->espConfig = espConfig;
    alexaStruct->gpioManager = gpioManager;
    amazonAlexa = new AmazonAlexa(alexaStruct, pwmAdcDataLocal);
    amazonAlexa->enable();
    amazonAlexa->addConfiguredDevices();
    //amazonAlexa->addDevice("Lamp one");
    isAlexaEnable = true;
  }

  /*adc1_config_width(ADC_WIDTH_BIT_12);
  adc1_config_channel_atten(ADC1_CHANNEL_6,ADC_ATTEN_DB_11);
  int val = adc1_get_raw(ADC1_CHANNEL_6);
  Serial.println("adc1_get_raw: " + String(val));*/

  Serial.println("Free size: " + String(ESP.getFreeSketchSpace()));
  Serial.println("Free Heap: " + String(ESP.getFreeHeap()));

}

void loop()
{

  if (isAlexaEnable)
  {
      amazonAlexa->handle();
  }

  // check for double reset every one second
  if ((millis() - lastTimeinMillisDoubleReset) > 1000)
  {
    doubleReset.handle();
    lastTimeinMillisDoubleReset = millis();
    /*int val = adc1_get_raw(ADC1_CHANNEL_6);
    Serial.println("adc1_get_raw: " + String(val));*/
  }

  // check for OTA every one second
  if ((millis() - lastTimeinMillisOta) > 1000)
  {
    otaHandler.handle();
    lastTimeinMillisOta = millis();
  }

  // Check for messages to MQTT Out
  if (mqttManagerOut != NULL)
  {
    if ((millis() - lastTimeinMillisMqtt) > 1000)
    {
      mqttManagerIn->handleMqtt();
      mqttManagerOut->handleMqtt();
      lastTimeinMillisMqtt = millis();
    }
    gpioManager->checkGpioChange(mqttManagerOut, pwmAdcDataLocal);
  }
  //delay(1000);
  yield();

}
