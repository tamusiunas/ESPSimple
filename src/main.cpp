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
            TOTALPWMSW, pinGpioMode, pinGpioStatusChanged, pinGpioDigitalStatus, dataStore);

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
    mqttManagerIn = new MqttManagerIn(mqttServerStr.c_str(), mqttPort, espConfig, gpioManager);
    mqttManagerIn->connect();
    mqttManagerOut = new MqttManagerOut(mqttServerStr.c_str(), mqttPort);
    mqttManagerOut->connect();
  }


  Serial.println("Configuring GPIO");
  gpioManager = new GpioManager(espConfig);
  //gpioManager = new GpioManager(espConfig);
  gpioManager->initializeGpio();

  Serial.print("Free size: ");
  Serial.println(ESP.getFreeSketchSpace());

  Serial.print("Free Heap: ");
  Serial.println(ESP.getFreeHeap());

}

void loop()
{

  unsigned long lastTimeinMillisDoubleReset = 0;
  unsigned long lastTimeinMillisOta = 0;
  unsigned long lastTimeinMillisMqtt = 0;

  //int valueRandrom = random(0,1023);
  //gpioManager->setPwm(12,valueRandrom);
  //valueRandrom = random(0,1023);
  //gpioManager->setPwm(14,valueRandrom);

  if ((millis() - lastTimeinMillisDoubleReset) > 1000)
  {
    doubleReset.handle();
    lastTimeinMillisDoubleReset = millis();
  }
  if ((millis() - lastTimeinMillisOta) > 1000)
  {
    otaHandler.handle();
    lastTimeinMillisOta = millis();
  }

  if (mqttManagerOut != NULL)
  {
    if ((millis() - lastTimeinMillisMqtt) > 1000)
    {
      mqttManagerIn->handleMqtt();
      mqttManagerOut->handleMqtt();
      lastTimeinMillisMqtt = millis();
    }
    
    for (int cont = 0; cont < TOTALGPIO ; cont++)
    {
      if (pinGpioStatusChanged[cont] == 1)
      {
        int pinGpioDigitalStatusLocal = pinGpioDigitalStatus[cont];
        Serial.println("Gpio status changed: " + String(cont) + ". New status: " + String(pinGpioDigitalStatus[cont]));
        String mqttKey[2];
        String mqttValue[2];
        mqttKey[0] = "gpio";
        mqttValue[0] = String(cont);
        mqttKey[1] = "status";
        if (pinGpioDigitalStatusLocal == HIGH)
        {
          mqttValue[1] = "high";
        }
        else
        {
          mqttValue[1] = "low";
        }
        pinGpioStatusChanged[cont] = 0;
        mqttManagerOut->publishMessageJson(mqttKey, mqttValue, 2, "GpioInfo");
      }
    }

    // TO IMPLEMENT A POLLING SYSTEM TO SEND MQTT MESSAGES WITH GPIO STATUS CHANGE
  }
  //delay(1000);
  yield();

}
