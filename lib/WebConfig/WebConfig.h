/**
 * @file WebConfig.h
 * @author Fabricio Tamusiunas (fabricio@tamusiunas.com)
 * @brief Web config management
 * @version 0.1
 * @date 2019-02-09
 * 
 * @copyright Copyright (c) 2019
 * 
 */

#ifndef WebConfig_h
#define WebConfig_h

#include "pgmspace.h"
#include "WebVariable.h"
#include "WebStatic.h"
#include "DataStore.h"
#include "WiFiAliases.h"
#include "WebServerAliases.h"
#include <DNSServer.h>
#include <Arduino.h>
#include "ESPConfig.h"
#include "SpiffsManager.h"

class WebConfig
{
public:
  /**
   * @brief Construct a new Web Config object
   * 
   * Hostname will be the Chip ID
   * 
   * @param espConfig ESPConfig
   * @param spiffsManager SpiffsManager
   */
  WebConfig(ESPConfig *espConfig, SpiffsManager *spiffsManager);

  /**
   * @brief Construct a new Web Config object
   * 
   * @param espConfig ESPConfig
   * @param hostName Hostname
   * @param spiffsManager SpiffsManager
   */
  WebConfig(ESPConfig *espConfig, const char *hostName, SpiffsManager *spiffsManager);

  /**
   * @brief Destroy the Web Config object
   * 
   */
  ~WebConfig();

  /**
   * @brief Start an SSID network listening to a TCP port (HTTP)
   * 
   * @param httpPort HTTP port to linten to
   * @param ipAddress Local IP address
   * @param ssid SSID
   * @param password Password
   * @return true Sucess
   * @return false Failure
   */
  bool startSsid(uint16_t httpPort, const char *ipAddress, const char *ssid, const char *password);

  /**
   * @brief Stop SSID
   * 
   * @return true True
   * @return false False
   */
  bool stopSsid();

  /**
   * @brief Hanlde HTTP request and process DNS queries
   * 
   */
  void processClient();

private:
  /**
   * @brief Add web pages handles
   * 
   */
  void handlePages();

  /**
   * @brief Hanlde GPIO web page
   * 
   */
  void handleGpio();

  /**
   * @brief Hanlde GPIO process and store data
   * 
   */
  void handleGpioConfig();

  /**
   * @brief Hanlde WiFi web page
   * 
   */
  void handleWifi();

  /**
   * @brief Handle WiFi process and store data
   * 
   */
  void handleWifiConfig();

  /**
   * @brief Handle exit and reboot
   * 
   */
  void handleExit();

  /**
   * @brief Handle WiFi refresh
   * 
   */
  void handleWifiRefresh();

  /**
   * @brief Handle styles.css (bootstrap)
   * 
   */
  void handleStylesCss();

  /**
   * @brief Handle jquery-3.3.1.slim.min.js
   * 
   */
  void handleJQuery();

  /**
   * @brief Handle popper.min.js
   * 
   */
  void handlePopper();

  /**
   * @brief Handle bootstrap.min.js
   * 
   */
  void handleBootStrap();

  /**
   * @brief Handle actions web page
   * 
   */
  void handleActions();

   /**
   * @brief Handle actions process and store data
   * 
   */
  void handleActionsConfig();

  /**
   * @brief Handle management web page
   * 
   */
  void handleManagement();

  /**
   * @brief Handle management process and store data
   * 
   */
  void handleManagementConfig();

  /**
   * @brief Handle Telegram web page
   * 
   */
  void handleTelegram();

  /**
   * @brief Handle telegram process and store data
   * 
   */
  void handleTelegramConfig();

  /**
   * @brief Handle ADC web page
   * 
   */
  void handleAdc();

  /**
   * @brief Handle ADC process and store data
   * 
   */
  void handleAdcConfig();

  /**
   * @brief Handle PWM (Pulse Width Modulation) web page
   * 
   */
  void handlePwm();

  /**
   * @brief Handle PWM (Pulse Width Modulation) process and store data
   * 
   */
  void handlePwmConfig();

  /**
   * @brief Handle components web page
   * 
   */
  void handleComponents();

  /**
   * @brief Handle components process and store data
   * 
   */
  void handleComponentsConfig();

  /**
   * @brief Handle page not found
   * 
   */
  void handleNotFound();

  /**
   * @brief Scan for WiFi networks
   * 
   */
  void scanWifi();

  /**
   * @brief Send content to be send to web browser
   * 
   * @param fieldCChar Content to be send to web browser
   */
  void sendResponseOk(const char * fieldCChar);

  String getGpioInOutAdcRowSelect(int gpioNumber);
  String getGpioInputOptions(int ignoreGpioInt, String configParameterStr);
  String getGpioInputOutputOptions(int ignoreGpioInt, String configParameterStr);
  String getGpioOutputOptions(int ignoreGpioInt, String configParameterStr);
  String getGpioActionRow(int gpioNumber);
  String getGpioAdcRow(int gpioNumber);
  String getGpioAdcOptions(int ignoreGpioInt, String configParameterStr);
  String getZeroCrossFrequencyOptions();
  String getGpioActionJS();
  String getPwmBody(String indexPwmStr);
  String getPwmZeroCrossOptions(String zeroCrossConfigured); 
  String getStringFormatted(String stringReceived);
  String getActionOptions(String actionOptionConfigured);
  String getAdcAnalisisTypeOptions(String adcAnalisisTypeConfigured);
  String getOnOffReversedOption(String adcAnalisisTypeConfigured);
  String getOnOffReverseOption(String digitalAnalisisTypeConfigured);
  String getActionDigitalBody(String indexDigital);
  String getActionAdcBody(String indexAdc);
  String getYesNoOptions(String configuredParameter);
  String getTelegramBody(String);
  String getComponentDht(String indexDht);
  String getComponentBmp180(String indexBmp180);
  String getComponentMcp3008(String indexMcp3008);
  String getComponentSsd160(String indexSsd160);
  String getComponentSsd160Frame(String indexSsd160Frame);

  int _httpPort;
  ESPConfig *_espConfig;
  const byte DNS_PORT = 53;
  IPAddress _ipAddress;
  DNSServer *_dnsServer;
  #ifdef ESP32
  #define WebServerLocal WebServer
  #define WifiSetHostname WiFi.setHostname
  #endif
  #ifdef ESP8266
  #define WebServerLocal ESP8266WebServer
  #define WifiSetHostname WiFi.hostname
  #endif
  WebServerLocal *_webServer;
  bool isConfig = true;
  String _wifiListHtmlOptions = "";
  String _hostName;
  String _chipId;
  SpiffsManager *_spiffsManager;
  unsigned long timeHandler = 0;
};



#endif
