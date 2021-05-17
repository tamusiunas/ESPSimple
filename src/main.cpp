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
  }  

  DataStore *dataStore = new DataStore(); // initialize DataStore
  SpiffsManager *spiffsManager = new SpiffsManager("/config.json",true,dataStore);
  spiffsManager->readFile();
  dataStore->printParameters(); // print Parameters read from spiffsManager

  // When to reverse an Action Digital
  volatile unsigned long *gpioDigitalActionIndexWhenReverseInMillis;
  int totalDigitalAction = String(dataStore->getValue("action_digital_total")).toInt();
  gpioDigitalActionIndexWhenReverseInMillis = (volatile unsigned long*)malloc(sizeof(volatile unsigned long) * totalDigitalAction);
  for (int cont = 0 ; cont < totalDigitalAction ; cont++)
  {
    gpioDigitalActionIndexWhenReverseInMillis[cont] = 0;
  }

  // Initialize espConfig with predefined parameters
  espConfig = new ESPConfig(pinGpioArray, pinGpioAvaliable, pinGpioAdcChannelArray, pinGpioAdcNumberArray,
            pinGpioInOut, pinGpioDesc, pinPwmValue, TOTALGPIO, pwmChannelGpioHw, TOTALPWMHW, pwmChannelGpioSw,
            TOTALPWMSW, pinGpioMode, pinGpioDigitalStatusChanged, pinGpioDigitalStatus, pinGpioAdcValue, 
            pinGpioAdcPreviousValue, pinPwmEnable, pinGpioPwmStatusChanged, pinGpioPwmStatus, pinAnalogOnly, 
            gpioDigitalActionIndexWhenReverseInMillis, dataStore);

  WebConfig webConfig(espConfig,spiffsManager);
  if (mustStartWebConfig)
  {
    webConfig.startSsid(80,"192.168.4.1",NULL,NULL);
    webConfig.processClient();
  }

  wiFiSTIManager = WiFiSTIManager(espConfig);
  wiFiSTIManager.start();

  /*timeClient = new NTPClient(ntpUDP,"pool.ntp.org",0,1800);
  timeClient->begin();
  timeClient->update();*/

  int syslogPort = 514;
  if (strcmp(dataStore->getValue("syslog_port"),""))
  {
    syslogPort = String(dataStore->getValue("syslog_port")).toInt();
  }
  const char* syslogIpAddress = dataStore->getValue("syslog_ip_address");
  //Check if syslog IP address is blank
  if (strcmp(syslogIpAddress,""))
  {
    Serial.println("Configuring Syslog");
    DebugMessage::syslogManager = new SyslogManager(dataStore->getValue("syslog_ip_address"), syslogPort);
    debugMessage = DebugMessage();
    debugMessage.debug("DEBUG started");
  }
  else
  {
    debugMessage = DebugMessage();
  }

  otaHandler = new OTAHandler();
  otaHandler->setEspConfig(espConfig);
  otaHandler->start(); 

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
  pwmAdcDataLocal->sendAdcGpioValue = sendAdcGpioValue;
  pwmAdcDataLocal->sendAdcAnalogOnlyValue = sendAdcAnalogOnlyValue;
  pwmAdcDataLocal->pinAnalogOnlyValue = pinAnalogOnlyValue;
  pwmAdcDataLocal->pinAnalogOnlyPreviousValue = pinAnalogOnlyPreviousValue;
  pwmAdcDataLocal->pinGpioAdcLastAction = pinGpioAdcLastAction;
  pwmAdcDataLocal->pinAnalogOnlyLastAction = pinAnalogOnlyLastAction;
  pwmAdcDataLocal->totalGPIO = TOTALGPIO;
  pwmAdcDataLocal->totalPwmHw = TOTALPWMHW;
  pwmAdcDataLocal->totalPwmSw = TOTALPWMSW;
  pwmAdcDataLocal->totalDht = 0;

  int componentDhtTotalInt = String(dataStore->getValue("component_dht_total")).toInt();
  if (componentDhtTotalInt > 0)
  {
    pwmAdcDataLocal->totalDht = componentDhtTotalInt;
    dhtManagerArray = (DhtManager**)malloc(sizeof(DhtManager*) * componentDhtTotalInt);
    pwmAdcDataLocal->sendDhtCelsius = (bool *)malloc(sizeof(bool *) * componentDhtTotalInt);
    pwmAdcDataLocal->sendDhtFahrenheit = (bool *)malloc(sizeof(bool *) * componentDhtTotalInt);
    pwmAdcDataLocal->sendDhtHumidity = (bool *)malloc(sizeof(bool *) * componentDhtTotalInt);
    for (int cont = 0; cont < componentDhtTotalInt ; cont++)
    {
      String componentDhtGpioStr = "component_dht_gpio_" + String(cont);
      int componentDhtGpioInt = String(dataStore->getValue(componentDhtGpioStr.c_str())).toInt();
      String componentDhtTypeStr = "component_dht_type_" + String(cont);
      String componentDhtTypeValueStr = String(dataStore->getValue(componentDhtTypeStr.c_str()));
      int dhtTypeInt = DHT11;
      if (componentDhtTypeValueStr == "dht22")
      {
        dhtTypeInt = DHT22;
      }
      pwmAdcDataLocal->sendDhtCelsius[cont] = false;
      pwmAdcDataLocal->sendDhtFahrenheit[cont] = false;
      pwmAdcDataLocal->sendDhtHumidity[cont] = false;
      dhtManagerArray[cont] = new DhtManager(componentDhtGpioInt, dhtTypeInt);
    }
  }
  else
  {
    debugMessage.debug("There's no DHT configured");
  }


  int actionAdcTotal = String(dataStore->getValue("action_adc_total")).toInt();
  pwmAdcDataLocal->adcActionIndexLastTimeInMillis = (volatile unsigned long *)malloc(sizeof(volatile unsigned long) * actionAdcTotal);
  pwmAdcDataLocal->adcActionWhenReverseInMillis = (volatile unsigned long *)malloc(sizeof(volatile unsigned long) * actionAdcTotal);
  for (int cont = 0 ; cont < actionAdcTotal ; cont ++)
  {
    pwmAdcDataLocal->adcActionIndexLastTimeInMillis[cont] = 0;
    pwmAdcDataLocal->adcActionWhenReverseInMillis[cont] = 0;
  }

  debugMessage.debug("Configuring MQTT");
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
    debugMessage.debug("mqttServerStr: " + mqttServerStr);
    debugMessage.debug("mqttPort: " + String(mqttPort));
    mqttManagerIn = new MqttManagerIn(mqttServerStr.c_str(), mqttPort, espConfig, gpioManager, pwmAdcDataLocal);
    mqttManagerIn->connect();
    mqttManagerOut = new MqttManagerOut(mqttServerStr.c_str(), mqttPort);
    mqttManagerOut->connect();
  }

  debugMessage.debug("Configuring GPIO");
  gpioManager = new GpioManager(espConfig);
  //gpioManager = new GpioManager(espConfig);
  gpioManager->initializeGpio();

  if (strcmp(dataStore->getValue("alexa_enable"),"yes") == 0)
  {
    debugMessage.debug("Configuring Alexa");
    alexaStruct = (AlexaStruct *)malloc(sizeof(AlexaStruct));
    alexaStruct->espConfig = espConfig;
    alexaStruct->gpioManager = gpioManager;
    amazonAlexa = new AmazonAlexa(alexaStruct, pwmAdcDataLocal);
    amazonAlexa->enable();
    amazonAlexa->addConfiguredDevices();
    isAlexaEnable = true;
  }

  checkActionMqtt = new CheckActionMqtt(mqttManagerOut, pwmAdcDataLocal, gpioManager, espConfig);

  debugMessage.debug("WifiGetChipId(): " + String(WifiGetChipId()));
  debugMessage.debug("Free size: " + String(ESP.getFreeSketchSpace()));
  debugMessage.debug("Free Heap: " + String(ESP.getFreeHeap()));

  //rf_driver.init();
 
}

void loop()
{

  if (isAlexaEnable)
  {
      amazonAlexa->handle();
  }

  #ifdef SYSLOGDEBUG
    if (wiFiSTIManager.isConnected())
    {
      debugMessage.handleSyslog();
    }
  #endif

  // check for ntp reset each second
  /*if ((millis() - lastTimeinMillisNtp) > 1000)
  {
    timeClient->update();
    // zqdebugMessage->debug(" <-- Now");
    lastTimeinMillisNtp = millis();
  }*/

  // check for double reset each second
  if ((millis() - lastTimeinMillisDoubleReset) > 1000)
  {
    doubleReset.handle();
    lastTimeinMillisDoubleReset = millis();
  }

  // check for OTA each second
  if ((millis() - lastTimeinMillisOta) > 1000)
  {
    otaHandler->handle();
    lastTimeinMillisOta = millis();
  }

  if ((millis() - lastTimeinMillisDht) > 5000)
  {
    for (int cont = 0; cont < pwmAdcDataLocal->totalDht ; cont++)
    {
      dhtManagerArray[cont]->update();
    }
    lastTimeinMillisDht = millis();
  }

  if ((millis() - lastTimeinMillisAdc) > 500)
  {
    //gpioManager->checkAdcReverse(pwmAdcDataLocal);
    checkActionMqtt->checkAdcReverse();
    //gpioManager->checkGpioDigitalReverse();
    checkActionMqtt->checkGpioDigitalReverse();
    //gpioManager->checkAdcGpioActions(mqttManagerOut, pwmAdcDataLocal);
    checkActionMqtt->checkAdcGpioActions();
    lastTimeinMillisAdc = millis();
  }

  // Check for messages to MQTT Out
  if (mqttManagerOut != NULL)
  {
    if ((millis() - lastTimeinMillisMqtt) > 1000)
    {
      mqttManagerIn->handleMqtt();
      mqttManagerOut->handleMqtt();
      checkActionMqtt->checkDht(dhtManagerArray);
      lastTimeinMillisMqtt = millis();
    }
    gpioManager->checkGpioChange(mqttManagerOut, pwmAdcDataLocal);

  }
  yield();

}
