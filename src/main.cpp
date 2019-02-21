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
            TOTALPWMSW, dataStore);

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
  if (String(dataStore->getValue("mqtt_ip_address")) != "")
  {
    int mqttPort = 1883;
    if (String(dataStore->getValue("mqtt_ip_address")) != "")
    {
      mqttPort = String(dataStore->getValue("mqtt_ip_address")).toInt();
    }
    mqttManagerIn = new MqttManagerIn(dataStore->getValue("mqtt_ip_address"), mqttPort, espConfig, gpioManager);
    mqttManagerIn->connect();
    mqttManagerOut = new MqttManagerOut(dataStore->getValue("mqtt_ip_address"), mqttPort);
    mqttManagerOut->connect();
  }
  

  Serial.println("Configuring GPIO");
  gpioManager = new GpioManager(espConfig, mqttManagerOut);
  //gpioManager = new GpioManager(espConfig);
  gpioManager->initializeGpio();

  Serial.print("Free size: ");
  Serial.println(ESP.getFreeSketchSpace());

  Serial.print("Free Heap: ");
  Serial.println(ESP.getFreeHeap());

}

void loop()
{

  //int valueRandrom = random(0,1023);
  //gpioManager->setPwm(12,valueRandrom);
  //valueRandrom = random(0,1023);
  //gpioManager->setPwm(14,valueRandrom);

  doubleReset.handle();
  otaHandler.handle();
  if (mqttManagerIn != NULL)
  {
    mqttManagerIn->handleMqtt();
    mqttManagerOut->handleMqtt();
    JsonManager *jsonManager = new JsonManager();
    String mqttKey[3];
    String mqttValue[3];
    mqttKey[0] = "Key0";
    mqttKey[1] = "Key1";
    mqttKey[2] = "Key2";
    mqttValue[0] = "Value0";
    mqttValue[1] = "Value1";
    mqttValue[2] = "Value2";
    //String jsonStr = jsonManager->formatJson(mqttKey,mqttValue,3);
    mqttManagerOut->publishMessage(jsonManager->formatJson(mqttKey,mqttValue,3,"Title"));
    //mqttManagerOut->publishMessage("Mensagem de Teste");
  }
  delay(1000);
  yield();

}
