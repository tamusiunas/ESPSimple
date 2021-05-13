# ESPSimple

ESPSimple is a system written in C++ that allows the configuration of Espressif microcontrollers (ESP8266 and ESP32) in a simple way through an intuitive WEB interface that allows even those who do not know programming to program it.

Some features:

 - Configuration via WEB Interface adapted to multiple screen resolutions based on bootstrap framework
 - Ability to enter setup mode as a WiFi and WEB server from double click on reset button or GPIO configured.
 - Ability to use the MQTT protocol for GPIO state verification and actions 
 - 

### How to compile it

```bash
git clone https://github.com/esp
cd ESPSimple
platformio run
```

### Hot up upload

```bash
cd ESPSimple
platformio run --target upload
```

Requirements to use 
