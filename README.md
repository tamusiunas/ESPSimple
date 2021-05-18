![ESPSimple logo](http://192.168.1.110:8080/ESPSimple_logo_640_2.png)

ESPSimple is a system written in C++ that allows the configuration of Espressif microcontrollers (ESP8266 and ESP32) in a simple way through an intuitive WEB interface allowing even those who do not know programming to program it.

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
- Analog GPIO pins can use a GPIO VREF pin and work from 1.1v to 3.9v (ESP32 only)
- Debug log can be sent to Syslog server (UDP)
- Full integration with Amazon Alexa for actions
- Full OTA (Over the Air) update support
- Can be integrated with Apple Homekit using [Homebridge](https://homebridge.io) and MQTT
- Other features coming soon...

### Platformio

This system is been developed using [PlatformIO](https://platformio.org).
  
To use it download and install PlatformIO at [https://platformio.org/platformio-ide](https://platformio.org/platformio-ide)

### How to compile ESPSimple

```bash
git clone https://github.com/esp
cd ESPSimple
# Edit platformio.inf informing about your board
platformio run
```

### How up upload ESPSimple to the board (ESP32 or ESP8266)

```bash
cd ESPSimple
# Edit platformio.inf informing about your board
platformio run --target upload
```

## Starting the configuration

To start the configuration interface:

- Press "reset" button twice (with one second between each one)
- Wait 5 seconds
- Search for an Wi-Fi network called "IOT-XXXXXXXXXX" where XXXXXXXXXX is the board ID
- Connect to the network and access the URL [http://192.168.4.1](https://192.168.4.1)

## WiFi config

###This tab allows to configure the Wi-Fi access.

- **Access Point** 
 - Choose your network - If it's not hidden and not present here, click on Refresh Network button. 
 - If the network is configured as hidden, enter the network name on the field bellow.
- **Password**: Network password
- **IP Config (optional)** - Use these fields if you are using static address (not using DHCP)
 - IP address: IP address
 - IP mask: IP mask
 - Gateway: Gateway IP address
 - DNS: DNS server
 
![Wi-Fi config](http://192.168.1.110:8080/ESPSimple_wifi.png)

## Management config

This tab is used to configure services

- **MQTT** - MQTT is actually the standard for IoT messaging. It can be used to send and receive messages from the device (board) to a message broker. It can be used to send actions and receive status change from the device. 
 - IP address: MQTT IP address.
 - Port: TCP port (default is 1883).

- **Syslog** - Syslog is a implementation for [RFC 5424](https://www.rfc-editor.org/rfc/rfc5424.txt). It's intended to send log messages to be registered in a remote device.
 - IP address: Syslog IP address.
 - Port: UDP port (default is 554).

- **OTA** - Over-the-Air update system. It's used by ESP32 and ESP8266 to update devices remotely.
 - Enable OTA: Yes or No.
 - OTA Password: Password for OTA update.

- **Web config GPIO** - Enable a GPIO (INPUT) to be used to start the configuration WEB service when on.
 - Web config trigger GPIO: GPIO (DIGITAL INPUT PULLUP) to be used to start the WEB interface when turned "On".
 - GPIO indicating configuration: GPIO (DIGITAL OUTPUT) that will have the status configured when available the WEB interface for config.
 - GPIO indicating configuration status: Status for the GPIO indicating the availability of the WEB interface.
 
![Management config](http://192.168.1.110:8080/ESPSimple_management.png)

## GPIO config

This tab is used to configure the GPIO available.

- **Mode**: Depending the GPIO the modes can be:
 - INPUT - Configure the GPIO as DIGITAL INPUT.
 - OUTPUT - Configure the GPIO as DIGITAL OUTPUT.
 - INPUT_PULLUP - Configure the GPIO as DIGITAL INPUT configuring it to create a system interruption when it status is changed (ON/OFF). This mode is MANDATORY for GPIO used as INPUT in Digital Actions. 
 - ADC - Configure the GPIO as an INPUT Analog-to-Digital mode. It's used to monitor the voltage variation on the GPIO.  
- **ADC attenuation**: The attenuation applied for GPIO when it's configured as ADC. This must be configured depending the input voltage, from 1.1v to 3.9v. (ESP32 only)
- **Comment**: Notes for the GPIO configuration.

![GPIO config](http://192.168.1.110:8080/ESPSimple_gpio_cut.pdf)

## Components config

### Sensors

- **Add DHT(11/22)**: Add a new DHT11/DHT22 sensor (to measure environmental temperature and humidity).
- **DHT Type**: DHT11 or DTH22.
- **DHT GPIO**: GPIO INPUT which DHT sensor is connected.

![Components config](http://192.168.1.110:8080/ESPSimple_components.png)

## Actions config

This tab allow to configure actions (digital and analog) to be generated when a digital input GPIO is changed or a configured amount of voltage is changed in an analog GPIO INPUT.

### Actions (Digital)

- **GPIO**: INPUT GPIO to be analysed (must be configured as INPUT_PULLUP on GPIO config tab).
- **Add Action (Digital)**: add an action based on a digital GPIO
- **On event**: Event analysed
 - On: act when GPIO is changed to on
 - Off: act when GPIO is changed off
 - Reversed: act when GPIO is changed to a different status
- **Action**: action performed when event matches
 - Reverse Voltage: invert the GPIO target status
 - Turn On: turn on GPIO target status
 - Turn Off: turn off GPIO target status
- **Time before action reversal (ms)**:Time in milliseconds before revert the action performed. Note: blank or zero disable this feature.
- **GPIO target**: OUTPUT GPIO target for the action 
- **Waiting Time Before Rearming Trigger (ms)**: Waiting time in milliseconds before this action can be performed again.

### Actions (ADC - Analog-to-Digital)

- **GPIO**: INPUT GPIO to be analysed (must be configured as ADC).
- **Add Action (ADC)**: add an action based on an ADC GPIO
- **On event**: Event analysed
 - Variation: act when GPIO is changed to on
 - Greater Than: act when GPIO is changed off
 - Lower Than: act when GPIO is changed to a different status
- **Action**: action performed when event matches
 - Reverse Voltage: invert the GPIO target status
 - Turn On: turn on GPIO target status
 - Turn Off: turn off GPIO target status
- **Time before action reversal (ms)**:Time in milliseconds before revert the action performed. Note: blank or zero disable this feature.
- **GPIO target**: OUTPUT GPIO target for the action 
- **Waiting Time Before Rearming Trigger (ms)**: Waiting time in milliseconds before this action can be performed again.


![Actions config](http://192.168.1.110:8080/ESPSimple_actions.png)

## ADC GPIO VREF config

![ADC GPIO VREF config](http://192.168.1.110:8080/ESPSimple_adc.png)

## Alexa config

![Alexa config](http://192.168.1.110:8080/ESPSimple_alexa.png)

## 
