# ESPSimple

ESPSimple is a system written in C++ that allows the configuration of Espressif microcontrollers (ESP8266 and ESP32) in a simple way through an intuitive WEB interface that allows even those who do not know programming to program it.

This is a system that can be used by different types of audiences, both by those who have good knowledge of electronics and for those who want to learn IoT without having the need to program in C, C++ or Python.

Some features:

- All configuration can be done via WEB Interface adapted to multiple screen resolutions based on bootstrap framework
- Users don't need to know about development to use it
- Ability to enter setup mode as a WiFi and WEB server from double click on reset button or GPIO configured.
- MQTT protocol can be used to get GPIO state and set actions 
- All GPIO (General Purpose Intput/Output) pin can be used (when available by the board)
- Actions based on digital and analog events can be programmed
 - Analyse if GPIO status is on or off
 - Action can be on/off/reverse
 - Timer to return GPIO to original state can be configured
 - Timer to rearm event can be configured
- PWM (Pulse-Width Modulation) can be used to control DC motors or AC motors (with zero-cross and hardware implemented PWM).
- Support to get environmental temperature and humidity via DHT11/DHT22
- Analog GPIO can use a GPIO VREF pin and work from 1.1v to 3.9v (ESP32 only)
- Debug log can be sent to Syslog server (TCP and UDP)
- Full integration with Amazon Alexa for actions
- Full OTA (Over the Air) update support
- Can be integrated with Apple Homekit using [Homebridge](https://homebridge.io) and MQTT
- Other features coming soon...

### Platformio

This system is been developed using [PlatformIO](https://platformio.org).
  
To use it download and install PlatformIO at [https://platformio.org/platformio-ide](https://platformio.org/platformio-ide)

### How to compile it

```bash
git clone https://github.com/esp
cd ESPSimple
# Edit platformio.inf informing about your board
platformio run
```

### Hot up upload it the board

```bash
cd ESPSimple
# Edit platformio.inf informing about your board
platformio run --target upload
```

## Web config

## WiFi config

## Management config

## GPIO config

## Components config

## Actions Config

## ADC GPIO VREF config

## Alexa Config

## 
