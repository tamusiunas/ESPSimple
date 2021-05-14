/**
 * @file HandleWeb.cpp
 * @author Fabricio Tamusiunas (fabricio@tamusiunas.com)
 * @brief Handle web pages
 * @version 0.1
 * @date 2019-02-09
 * 
 * @copyright Copyright (c) 2019
 * 
 */

#include "WebConfig.h"

void WebConfig::handlePages()
{
  _webServer->on("/", std::bind(&WebConfig::handleGpio, this));
  _webServer->on("/exit.html", std::bind(&WebConfig::handleExit, this));
  _webServer->on("/gpio.html", std::bind(&WebConfig::handleGpio, this));
  _webServer->on("/gpioConfig", std::bind(&WebConfig::handleGpioConfig, this));
  _webServer->on("/wifi.html", std::bind(&WebConfig::handleWifi, this));
  _webServer->on("/wifiConfig", std::bind(&WebConfig::handleWifiConfig, this));
  _webServer->on("/wifi-refresh.html", std::bind(&WebConfig::handleWifiRefresh, this));
  _webServer->on("/actions.html", std::bind(&WebConfig::handleActions, this));
  _webServer->on("/actionsConfig", std::bind(&WebConfig::handleActionsConfig, this));
  _webServer->on("/management.html", std::bind(&WebConfig::handleManagement, this));
  _webServer->on("/managementConfig", std::bind(&WebConfig::handleManagementConfig, this));
  _webServer->on("/adc.html", std::bind(&WebConfig::handleAdc, this));
  _webServer->on("/adcConfig", std::bind(&WebConfig::handleAdcConfig, this));
  _webServer->on("/pwm.html", std::bind(&WebConfig::handlePwm, this));
  _webServer->on("/pwmConfig", std::bind(&WebConfig::handlePwmConfig, this));
  _webServer->on("/telegram.html", std::bind(&WebConfig::handleTelegram, this));
  _webServer->on("/telegramConfig", std::bind(&WebConfig::handleTelegramConfig, this));
  _webServer->on("/alexa.html", std::bind(&WebConfig::handleAlexa, this));
  _webServer->on("/alexaConfig", std::bind(&WebConfig::handleAlexaConfig, this));
  _webServer->on("/components.html", std::bind(&WebConfig::handleComponents, this));
  _webServer->on("/componentsConfig", std::bind(&WebConfig::handleComponentsConfig, this));
  _webServer->on("/styles.css", std::bind(&WebConfig::handleStylesCss, this));
  _webServer->on("/jquery-3.3.1.slim.min.js", std::bind(&WebConfig::handleJQuery, this));
  _webServer->on("/popper.min.js", std::bind(&WebConfig::handlePopper, this));
  _webServer->on("/bootstrap.min.js", std::bind(&WebConfig::handleBootStrap, this));
  #ifdef ENABLE_CAPTIVE_PORTAL
  _webServer->onNotFound(std::bind(&WebConfig::handleNotFound, this));
  #endif
}

void WebConfig::handleNotFound()
{
  _webServer->sendHeader("Location", "http://" + _ipAddress.toString(), true);
  _webServer->send(302, FPSTR(HTTP_HEAD_CT_TP), "");
  _webServer->client().stop();
}

void WebConfig::handleExit()
{
  String exitMessage = "<html><body><center><H2>Rebooting System</H2></center></body></html>";
  _webServer->setContentLength(exitMessage.length());
  _webServer->send(200, FPSTR(HTTP_HEAD_CT_HTML),"");
  _webServer->sendContent(exitMessage);
  _webServer->client().stop();
  delay(2000);
  ESP.restart();
}

void WebConfig::handleTelegram()
{
  int totalParameters = _espConfig->getDataStore()->getDataParametersCount();
  int telegramLength = 0;
  int totalTelegram = 0;
  for (int cont=0; cont<totalParameters; cont++)
  {
    if (_espConfig->getDataStore()->getParameterByPos(cont) != NULL)
    {
      String parameterLocal = _espConfig->getDataStore()->getParameterByPos(cont)->getField();
      if (parameterLocal.indexOf("telegram_message_r_") == 0)
      {
        ++totalTelegram;
        String telegramIndexStr = parameterLocal.substring(19);
        telegramLength += getTelegramBody(telegramIndexStr).length();
      }
    }
  }
  String tokenIdStr = getStringFormatted(_espConfig->getDataStore()->getValue("telegram_token_id"));
  String ClientIdStr = getStringFormatted(_espConfig->getDataStore()->getValue("telegram_client_id"));
  _webServer->setContentLength(strlen_P(HEADER_EN_US)+strlen_P(TELEGRAM_EN_US_P1)+getYesNoOptions("telegram_enable").length()+
                               strlen_P(TELEGRAM_EN_US_P2)+tokenIdStr.length()+strlen_P(TELEGRAM_EN_US_P3)+
                               ClientIdStr.length()+strlen_P(TELEGRAM_EN_US_P4)+telegramLength+
                               strlen_P(TELEGRAM_EN_US_P5)+strlen_P(FOOTER_EN_US_P1)+
                               strlen_P(TELEGRAM_JS_EN_US_P1)+String(totalTelegram).length()+strlen_P(TELEGRAM_JS_EN_US_P2)+getGpioInputOptions(-1,"").length()+
                               strlen_P(TELEGRAM_JS_EN_US_P3)+strlen_P(FOOTER_EN_US_P2));
  _webServer->send(200, FPSTR(HTTP_HEAD_CT_HTML),"");
  _webServer->sendContent_P(HEADER_EN_US);
  _webServer->sendContent_P(TELEGRAM_EN_US_P1);
  _webServer->sendContent(getYesNoOptions("telegram_enable"));
  _webServer->sendContent_P(TELEGRAM_EN_US_P2);
  _webServer->sendContent(tokenIdStr);
  _webServer->sendContent_P(TELEGRAM_EN_US_P3);
  _webServer->sendContent(ClientIdStr);
  _webServer->sendContent_P(TELEGRAM_EN_US_P4);
  for (int cont=0; cont<totalParameters; cont++)
  {
    if (_espConfig->getDataStore()->getParameterByPos(cont) != NULL)
    {
      String parameterLocal = _espConfig->getDataStore()->getParameterByPos(cont)->getField();
      if (parameterLocal.indexOf("telegram_message_r_") == 0)
      {
        String telegramIndexStr = parameterLocal.substring(19);
        _webServer->sendContent(getTelegramBody(telegramIndexStr));
      }
    }
  }
  _webServer->sendContent_P(TELEGRAM_EN_US_P5);
  _webServer->sendContent_P(FOOTER_EN_US_P1);
  _webServer->sendContent_P(TELEGRAM_JS_EN_US_P1);
  _webServer->sendContent(String(totalTelegram));
  _webServer->sendContent_P(TELEGRAM_JS_EN_US_P2);
  _webServer->sendContent(getGpioInputOptions(-1,""));
  _webServer->sendContent_P(TELEGRAM_JS_EN_US_P3);
  _webServer->sendContent_P(FOOTER_EN_US_P2);
}

void WebConfig::handleAlexa()
{
  int totalParameters = _espConfig->getDataStore()->getDataParametersCount();
  int alexaLength = 0;
  int totalAlexa = 0;
  //Serial.println("P1");
  for (int cont=0; cont<totalParameters; cont++)
  {
    if (_espConfig->getDataStore()->getParameterByPos(cont) != NULL)
    {
      String parameterLocal = _espConfig->getDataStore()->getParameterByPos(cont)->getField();
      //Serial.println(parameterLocal);
      if (parameterLocal.indexOf("alexa_device_name_r_") == 0)
      {
        //Serial.println("Found I1");
        ++totalAlexa;
        String alexaIndexStr = parameterLocal.substring(20);
        alexaLength += getAlexaBody(alexaIndexStr).length();
      }
    }
  }
  
  _webServer->setContentLength(strlen_P(HEADER_EN_US)+strlen_P(ALEXA_EN_US_P1)+getYesNoOptions("alexa_enable").length()+
                               strlen_P(ALEXA_EN_US_P2)+alexaLength+strlen_P(ALEXA_EN_US_P3)+strlen_P(FOOTER_EN_US_P1)+
                               strlen_P(ALEXA_JS_EN_US_P1)+String(totalAlexa).length()+strlen_P(ALEXA_JS_EN_US_P2)+
                               getGpioInputOptions(-1,"").length()+strlen_P(ALEXA_JS_EN_US_P3)+strlen_P(FOOTER_EN_US_P2));

  _webServer->send(200, FPSTR(HTTP_HEAD_CT_HTML),"");
  _webServer->sendContent_P(HEADER_EN_US);
  _webServer->sendContent_P(ALEXA_EN_US_P1);
  _webServer->sendContent(getYesNoOptions("alexa_enable"));
  _webServer->sendContent_P(ALEXA_EN_US_P2);
  for (int cont=0; cont<totalParameters; cont++)
  {
    if (_espConfig->getDataStore()->getParameterByPos(cont) != NULL)
    {
      String parameterLocal = _espConfig->getDataStore()->getParameterByPos(cont)->getField();
      if (parameterLocal.indexOf("alexa_device_name_r_") == 0)
      {
        String alexaIndexStr = parameterLocal.substring(20);
        _webServer->sendContent(getAlexaBody(alexaIndexStr));
      }
    }
  }
  _webServer->sendContent_P(ALEXA_EN_US_P3);
  _webServer->sendContent_P(FOOTER_EN_US_P1);
  _webServer->sendContent_P(ALEXA_JS_EN_US_P1);
  _webServer->sendContent(String(totalAlexa));
  _webServer->sendContent_P(ALEXA_JS_EN_US_P2);
  _webServer->sendContent(getGpioInputOptions(-1,""));
  _webServer->sendContent_P(ALEXA_JS_EN_US_P3);
  _webServer->sendContent_P(FOOTER_EN_US_P2);
}

void WebConfig::handleComponents()
{
  int totalParameters = _espConfig->getDataStore()->getDataParametersCount();
  int dhtLength = 0;
  int totalDht = 0;
  int bmp180Length = 0;
  int totalBmp180 = 0;
  int mcp3008Length = 0;
  int totalMcp3008 = 0;
  int ssd160Length = 0;
  int totalSsd160 = 0;
  int ssd160FramesLength = 0;
  int totalSsd160Frames = 0;

  for (int cont=0; cont<totalParameters; cont++)
  {
    if (_espConfig->getDataStore()->getParameterByPos(cont) != NULL)
    {
      String parameterLocal = _espConfig->getDataStore()->getParameterByPos(cont)->getField();
      if (parameterLocal.indexOf("component_dht_gpio_") == 0)
      {
        ++totalDht;
        String dhtStr = parameterLocal.substring(19);
        String dhtTypeStr = "component_dht_type_" + dhtStr;
        String dhtTypeValueStr = String(_espConfig->getDataStore()->getValue(dhtTypeStr.c_str()));
        dhtLength += getComponentDht(dhtStr,dhtTypeValueStr).length();
      }
      #ifdef ENABLE_BMP180
      else
      if (parameterLocal.indexOf("component_bmp180_sda_") == 0)
      {
        ++totalBmp180;
        String bmp180Str = parameterLocal.substring(21);
        bmp180Length += getComponentBmp180(bmp180Str).length();
      }
      #endif
      #ifdef ENABLE_MCP3008
      else
      if (parameterLocal.indexOf("component_mcp3008_clock_") == 0)
      {
        ++totalMcp3008;
        String mcp3008Str = parameterLocal.substring(24);
        mcp3008Length += getComponentMcp3008(mcp3008Str).length();
      }
      #endif
      #ifdef ENABLE_SSD160
      else
      if (parameterLocal.indexOf("component_ssd160_sda_") == 0)
      {
        ++totalSsd160;
        String ssd160Str = parameterLocal.substring(21);
        ssd160Length += getComponentSsd160(ssd160Str).length();
      }
      else
      if (parameterLocal.indexOf("component_ssd160_frame_type_") == 0)
      {
        ++totalSsd160Frames;
        String ssd160frameStr = parameterLocal.substring(28);
        ssd160FramesLength += getComponentSsd160Frame(ssd160frameStr).length();
      }
      #endif
    }
  }

  _webServer->setContentLength(strlen_P(HEADER_EN_US)+strlen_P(COMPONENTS_EN_US_P1)+dhtLength+strlen_P(COMPONENTS_EN_US_P2)+
                               strlen_P(COMPONENTS_EN_US_P3)+bmp180Length+strlen_P(COMPONENTS_EN_US_P4)+mcp3008Length+strlen_P(COMPONENTS_EN_US_P5)+
                               strlen_P(COMPONENTS_EN_US_P6)+ssd160Length+strlen_P(COMPONENTS_EN_US_P7)+ssd160FramesLength+strlen_P(COMPONENTS_EN_US_P8)+
                               strlen_P(FOOTER_EN_US_P1)+
                               strlen_P(COMPONENTS_JS_EN_US_P1)+String(totalDht).length()+strlen_P(COMPONENTS_JS_EN_US_P2)+
                               String(totalBmp180).length()+strlen_P(COMPONENTS_JS_EN_US_P3)+String(totalMcp3008).length()+
                               strlen_P(COMPONENTS_JS_EN_US_P4)+String(totalSsd160).length()+strlen_P(COMPONENTS_JS_EN_US_P5)+
                               String(totalSsd160Frames).length()+strlen_P(COMPONENTS_JS_EN_US_P6)+getGpioInputOptions(-1,"").length()+
                               strlen_P(COMPONENTS_JS_EN_US_P7)+strlen_P(COMPONENTS_JS_EN_US_P8)+getGpioInputOutputOptions(-1,"").length()+
                               strlen_P(COMPONENTS_JS_EN_US_P9)+getGpioOutputOptions(-1,"").length()+
                               strlen_P(COMPONENTS_JS_EN_US_P10)+strlen_P(COMPONENTS_JS_EN_US_P11)+getGpioOutputOptions(-1,"").length()+
                               strlen_P(COMPONENTS_JS_EN_US_P12)+getGpioInputOutputOptions(-1,"").length()  +
                               strlen_P(COMPONENTS_JS_EN_US_P13)+getGpioOutputOptions(-1,"").length()+
                               strlen_P(COMPONENTS_JS_EN_US_P14)+getGpioOutputOptions(-1,"").length()+strlen_P(COMPONENTS_JS_EN_US_P15)+
                               strlen_P(COMPONENTS_JS_EN_US_P16)+getGpioInputOutputOptions(-1,"").length()+strlen_P(COMPONENTS_JS_EN_US_P17)+
                               getGpioOutputOptions(-1,"").length()+strlen_P(COMPONENTS_JS_EN_US_P18)+
                               strlen_P(COMPONENTS_JS_EN_US_P19)+strlen_P(COMPONENTS_JS_EN_US_P20)+strlen_P(FOOTER_EN_US_P2));

  _webServer->send(200, FPSTR(HTTP_HEAD_CT_HTML),"");
  _webServer->sendContent_P(HEADER_EN_US);
  _webServer->sendContent_P(COMPONENTS_EN_US_P1);
  for (int cont=0; cont<totalParameters; cont++)
  {
    if (_espConfig->getDataStore()->getParameterByPos(cont) != NULL)
    {
      String parameterLocal = _espConfig->getDataStore()->getParameterByPos(cont)->getField();
      if (parameterLocal.indexOf("component_dht_gpio_") == 0)
      {
        String dhtStr = parameterLocal.substring(19);
        String dhtTypeStr = "component_dht_type_" + dhtStr;
        String dhtTypeValueStr = String(_espConfig->getDataStore()->getValue(dhtTypeStr.c_str()));
        _webServer->sendContent(getComponentDht(dhtStr, dhtTypeValueStr));
      }
    }
  }
  _webServer->sendContent_P(COMPONENTS_EN_US_P2);
  _webServer->sendContent_P(COMPONENTS_EN_US_P3);
  #ifdef ENABLE_BMP180
  for (int cont=0; cont<totalParameters; cont++)
  {
    if (_espConfig->getDataStore()->getParameterByPos(cont) != NULL)
    {
      String parameterLocal = _espConfig->getDataStore()->getParameterByPos(cont)->getField();
      if (parameterLocal.indexOf("component_bmp180_sda_") == 0)
      {
        String bmp180Str = parameterLocal.substring(21);
        _webServer->sendContent(getComponentBmp180(bmp180Str));
      }
    }
  }
  #endif
  _webServer->sendContent_P(COMPONENTS_EN_US_P4);
  #ifdef ENABLE_MCP3008
  for (int cont=0; cont<totalParameters; cont++)
  {
    if (_espConfig->getDataStore()->getParameterByPos(cont) != NULL)
    {
      String parameterLocal = _espConfig->getDataStore()->getParameterByPos(cont)->getField();
      if (parameterLocal.indexOf("component_mcp3008_clock_") == 0)
      {
        String mcp3008Str = parameterLocal.substring(24);
        _webServer->sendContent(getComponentMcp3008(mcp3008Str));
      }
    }
  }
  #endif
  _webServer->sendContent_P(COMPONENTS_EN_US_P5);
  _webServer->sendContent_P(COMPONENTS_EN_US_P6);
  #ifdef ENABLE_SSD160
  for (int cont=0; cont<totalParameters; cont++)
  {
    if (_espConfig->getDataStore()->getParameterByPos(cont) != NULL)
    {
      String parameterLocal = _espConfig->getDataStore()->getParameterByPos(cont)->getField();
      if (parameterLocal.indexOf("component_ssd160_sda_") == 0)
      {
        String ssd160Str = parameterLocal.substring(21);
        _webServer->sendContent(getComponentSsd160(ssd160Str));
      }
    }
  }
  #endif
  _webServer->sendContent_P(COMPONENTS_EN_US_P7);
  #ifdef ENABLE_SSD160
  for (int cont=0; cont<totalParameters; cont++)
  {
    if (_espConfig->getDataStore()->getParameterByPos(cont) != NULL)
    {
      String parameterLocal = _espConfig->getDataStore()->getParameterByPos(cont)->getField();
      if (parameterLocal.indexOf("component_ssd160_frame_type_") == 0)
      {
        String ssd160frameStr = parameterLocal.substring(28);
        _webServer->sendContent(getComponentSsd160Frame(ssd160frameStr));
      }
    }
  }
  #endif
  _webServer->sendContent_P(COMPONENTS_EN_US_P8);
  _webServer->sendContent_P(FOOTER_EN_US_P1);
  _webServer->sendContent_P(COMPONENTS_JS_EN_US_P1);
  _webServer->sendContent(String(totalDht));
  _webServer->sendContent_P(COMPONENTS_JS_EN_US_P2);
  _webServer->sendContent(String(totalBmp180));
  _webServer->sendContent_P(COMPONENTS_JS_EN_US_P3);
  _webServer->sendContent(String(totalMcp3008));
  _webServer->sendContent_P(COMPONENTS_JS_EN_US_P4);
  _webServer->sendContent(String(totalSsd160));
  _webServer->sendContent_P(COMPONENTS_JS_EN_US_P5);
  _webServer->sendContent(String(totalSsd160Frames));
  _webServer->sendContent_P(COMPONENTS_JS_EN_US_P6);
  _webServer->sendContent(getGpioInputOptions(-1,""));
  _webServer->sendContent_P(COMPONENTS_JS_EN_US_P7);
  _webServer->sendContent_P(COMPONENTS_JS_EN_US_P8);
  _webServer->sendContent(getGpioInputOutputOptions(-1,""));
  _webServer->sendContent_P(COMPONENTS_JS_EN_US_P9);
  _webServer->sendContent(getGpioOutputOptions(-1,""));
  _webServer->sendContent_P(COMPONENTS_JS_EN_US_P10);
  _webServer->sendContent_P(COMPONENTS_JS_EN_US_P11);
  _webServer->sendContent(getGpioOutputOptions(-1,""));
  _webServer->sendContent_P(COMPONENTS_JS_EN_US_P12);
  _webServer->sendContent(getGpioInputOutputOptions(-1,""));
  _webServer->sendContent_P(COMPONENTS_JS_EN_US_P13);
  _webServer->sendContent(getGpioOutputOptions(-1,""));
  _webServer->sendContent_P(COMPONENTS_JS_EN_US_P14);
  _webServer->sendContent(getGpioOutputOptions(-1,""));
  _webServer->sendContent_P(COMPONENTS_JS_EN_US_P15);
  _webServer->sendContent_P(COMPONENTS_JS_EN_US_P16);
  _webServer->sendContent(getGpioInputOutputOptions(-1,""));
  _webServer->sendContent_P(COMPONENTS_JS_EN_US_P17);
  _webServer->sendContent(getGpioOutputOptions(-1,""));
  _webServer->sendContent_P(COMPONENTS_JS_EN_US_P18);
  _webServer->sendContent_P(COMPONENTS_JS_EN_US_P19);
  _webServer->sendContent_P(COMPONENTS_JS_EN_US_P20);
  _webServer->sendContent_P(FOOTER_EN_US_P2);
}

void WebConfig::handlePwm()
{
  int totalParameters = _espConfig->getDataStore()->getDataParametersCount();
  int pwmLength = 0;
  int totalPwm = 0;
  for (int cont=0; cont<totalParameters; cont++)
  {
    if (_espConfig->getDataStore()->getParameterByPos(cont) != NULL)
    {
      String parameterLocal = _espConfig->getDataStore()->getParameterByPos(cont)->getField();
      if (parameterLocal.indexOf("pwm_gpio_r_") == 0)
      {
        ++totalPwm;
        String pwmIndexStr = parameterLocal.substring(11);
        pwmLength += getPwmBody(pwmIndexStr).length();
      }
    }
  }
  String pwmEnUsP1Str = FPSTR(PWM_EN_US_P1);
  pwmEnUsP1Str.replace("{::SELECTZEROCROSSFREQUENCY::}",getZeroCrossFrequencyOptions());
  pwmEnUsP1Str.replace("{::SELECTZEROCROSSENABLE::}",getYesNoOptions("pwm_enable_zero_cross"));

  _webServer->setContentLength(strlen_P(HEADER_EN_US)+pwmEnUsP1Str.length()+
                               getGpioInputOptions(-1,_espConfig->getDataStore()->getValue("pwm_zero_cross_gpio")).length()+
                               strlen_P(PWM_EN_US_P2)+pwmLength+strlen_P(PWM_EN_US_P3)+strlen_P(FOOTER_EN_US_P1)+
                               String(totalPwm).length()+strlen_P(PWM_JS_EN_US_P1)+strlen_P(PWM_JS_EN_US_P2)+
                               getGpioInputOptions(-1,"").length()+strlen_P(PWM_JS_EN_US_P3)+strlen_P(FOOTER_EN_US_P2));
  //_webServer->setContentLength(CONTENT_LENGTH_UNKNOWN);
  _webServer->send(200, FPSTR(HTTP_HEAD_CT_HTML),"");
  _webServer->sendContent_P(HEADER_EN_US);
  _webServer->sendContent(pwmEnUsP1Str);
  _webServer->sendContent(getGpioInputOptions(-1,_espConfig->getDataStore()->getValue("pwm_zero_cross_gpio")));
  _webServer->sendContent_P(PWM_EN_US_P2);


  for (int cont=0; cont<totalParameters; cont++)
  {
    if (_espConfig->getDataStore()->getParameterByPos(cont) != NULL)
    {
      String parameterLocal = _espConfig->getDataStore()->getParameterByPos(cont)->getField();
      if (parameterLocal.indexOf("pwm_gpio_r_") == 0)
      {
        String pwmIndexStr = parameterLocal.substring(11);
        Serial.println("(handlePwm) Found id: " + pwmIndexStr);
        _webServer->sendContent(getPwmBody(pwmIndexStr));
      }
    }
  }
  _webServer->sendContent_P(PWM_EN_US_P3);
  _webServer->sendContent_P(FOOTER_EN_US_P1);
  _webServer->sendContent_P(PWM_JS_EN_US_P1);
  _webServer->sendContent(String(totalPwm));
  _webServer->sendContent_P(PWM_JS_EN_US_P2);
  _webServer->sendContent(getGpioInputOptions(-1,""));
  _webServer->sendContent_P(PWM_JS_EN_US_P3);
  _webServer->sendContent_P(FOOTER_EN_US_P2);
  //_webServer->client().stop();
}

void WebConfig::handleAdc()
{
  String gpioAdcVref = _espConfig->getDataStore()->getValue("gpio_adc_vref");
  _webServer->setContentLength(strlen_P(HEADER_EN_US)+strlen_P(GPIO_ADC_EN_US_P1)+getGpioInputOptions(-1,gpioAdcVref.c_str()).length()+
                               strlen_P(GPIO_ADC_EN_US_P2)+strlen_P(FOOTER_EN_US_P1)+strlen_P(FOOTER_EN_US_P2));
  _webServer->send(200, FPSTR(HTTP_HEAD_CT_HTML),"");
  _webServer->sendContent_P(HEADER_EN_US);
  _webServer->sendContent_P(GPIO_ADC_EN_US_P1);
  _webServer->sendContent(getGpioInputOptions(-1,gpioAdcVref.c_str()));
  _webServer->sendContent_P(GPIO_ADC_EN_US_P2);
  _webServer->sendContent_P(FOOTER_EN_US_P1);
  _webServer->sendContent_P(FOOTER_EN_US_P2);
}

void WebConfig::handleManagement()
{

  String mqttIpAddressStr = getStringFormatted(_espConfig->getDataStore()->getValue("mqtt_ip_address"));
  String mqttPortStr = getStringFormatted(_espConfig->getDataStore()->getValue("mqtt_port"));
  String syslogIpAddressStr = getStringFormatted(_espConfig->getDataStore()->getValue("syslog_ip_address"));
  String syslogPortStr = getStringFormatted(_espConfig->getDataStore()->getValue("syslog_port"));
  String otaPasswordStr = getStringFormatted(_espConfig->getDataStore()->getValue("ota_password"));
  //String webConfigWaitingTimeStr = getStringFormatted(_espConfig->getDataStore()->getValue("web_config_waiting_time"));

  int mqttIpAddressLength = mqttIpAddressStr.length();
  int mqttPortLength = mqttPortStr.length();
  int syslogIpAddressLength = syslogIpAddressStr.length();
  int syslogPortLength = syslogPortStr.length();
  int enableOtaLength = getYesNoOptions("enable_ota").length();
  int otaPasswordLength = otaPasswordStr.length();
  //int webConfigWaitingTimeLength = webConfigWaitingTimeStr.length();

  _webServer->setContentLength(strlen_P(HEADER_EN_US)+strlen_P(MANAGEMENT_EN_US_P1)+mqttIpAddressLength+
                               strlen_P(MANAGEMENT_EN_US_P2)+mqttPortLength+strlen_P(MANAGEMENT_EN_US_P3)+
                               syslogIpAddressLength+strlen_P(MANAGEMENT_EN_US_P4)+syslogPortLength+
                               strlen_P(MANAGEMENT_EN_US_P5)+enableOtaLength+strlen_P(MANAGEMENT_EN_US_P6)+otaPasswordLength+strlen_P(MANAGEMENT_EN_US_P7)+
                               getNoneOption().length() + getGpioInputOptions(-1,_espConfig->getDataStore()->getValue("web_config_gpio")).length()+
                               strlen_P(MANAGEMENT_EN_US_P8)+
                               strlen_P(MANAGEMENT_EN_US_P9)+
                               getGpioOutputOptions(-1,_espConfig->getDataStore()->getValue("web_config_indicating")).length()+
                               strlen_P(MANAGEMENT_EN_US_P10)+
                               getOnOffOption(_espConfig->getDataStore()->getValue("web_config_indicating_status")).length()
                               +strlen_P(MANAGEMENT_EN_US_P11)+strlen_P(FOOTER_EN_US_P1)+strlen_P(FOOTER_EN_US_P2));
  _webServer->send(200, FPSTR(HTTP_HEAD_CT_HTML),"");
  _webServer->sendContent_P(HEADER_EN_US);
  _webServer->sendContent_P(MANAGEMENT_EN_US_P1);
  _webServer->sendContent(mqttIpAddressStr);
  _webServer->sendContent_P(MANAGEMENT_EN_US_P2);
  _webServer->sendContent(mqttPortStr);
  _webServer->sendContent_P(MANAGEMENT_EN_US_P3);
  _webServer->sendContent(syslogIpAddressStr);
  _webServer->sendContent_P(MANAGEMENT_EN_US_P4);
  _webServer->sendContent(syslogPortStr);
  _webServer->sendContent_P(MANAGEMENT_EN_US_P5);
  _webServer->sendContent(getYesNoOptions("enable_ota"));
  _webServer->sendContent_P(MANAGEMENT_EN_US_P6);
  _webServer->sendContent(otaPasswordStr);
  _webServer->sendContent_P(MANAGEMENT_EN_US_P7);
  _webServer->sendContent(getNoneOption());
  _webServer->sendContent(getGpioInputOptions(-1,_espConfig->getDataStore()->getValue("web_config_gpio")));
  _webServer->sendContent_P(MANAGEMENT_EN_US_P8);
  _webServer->sendContent_P(MANAGEMENT_EN_US_P9);
  _webServer->sendContent(getGpioOutputOptions(-1,_espConfig->getDataStore()->getValue("web_config_indicating")));
  _webServer->sendContent_P(MANAGEMENT_EN_US_P10);
  _webServer->sendContent(getOnOffOption(_espConfig->getDataStore()->getValue("web_config_indicating_status")));
  _webServer->sendContent_P(MANAGEMENT_EN_US_P11);
  _webServer->sendContent_P(FOOTER_EN_US_P1);
  _webServer->sendContent_P(FOOTER_EN_US_P2);
}

void WebConfig::handleGpio()
{
  int gpioSelectLenght = 0;
  for (int cont = 0; cont < _espConfig->getTotalGpio(); cont++)
  {
    gpioSelectLenght += getGpioInOutAdcRowSelect(cont).length();
  }
  _webServer->setContentLength(strlen_P(HEADER_EN_US)+strlen_P(GPIO_EN_US_P1)+gpioSelectLenght+strlen_P(GPIO_EN_US_P2)+
                               strlen_P(FOOTER_EN_US_P1)+strlen_P(FOOTER_EN_US_P2));
  _webServer->send(200, FPSTR(HTTP_HEAD_CT_HTML),"");
  _webServer->sendContent_P(HEADER_EN_US);
  _webServer->sendContent_P(GPIO_EN_US_P1);
  for (int cont = 0; cont < _espConfig->getTotalGpio(); cont++)
  {
    _webServer->sendContent(getGpioInOutAdcRowSelect(cont));
  }
  _webServer->sendContent_P(GPIO_EN_US_P2);
  _webServer->sendContent_P(FOOTER_EN_US_P1);
  _webServer->sendContent_P(FOOTER_EN_US_P2);
}

void WebConfig::handleWifiRefresh()
{
  scanWifi();
  handleWifi();
}

void WebConfig::handlePwmConfig()
{
  int indexRealPwm = 0;
  _espConfig->getDataStore()->deleteParameterPrefix("pwm_");
  _espConfig->getDataStore()->addParameter(new DataParameter("pwm_enable_zero_cross",_webServer->arg("pwm_enable_zero_cross").c_str()));
  _espConfig->getDataStore()->addParameter(new DataParameter("pwm_zero_cross_frequency",_webServer->arg("pwm_zero_cross_frequency").c_str()));
  _espConfig->getDataStore()->addParameter(new DataParameter("pwm_zero_cross_gpio",_webServer->arg("pwm_zero_cross_gpio").c_str()));
  for (int cont = 0; cont < _webServer->args(); cont++)
  {
    String argNameStr = _webServer->argName(cont);

    if (argNameStr.indexOf("pwm_gpio_r_") >= 0)
    {
      String pwmIndexStr = argNameStr.substring(11);

      String pwmGpioStr = _webServer->arg("pwm_gpio_r_" + pwmIndexStr);
      String pwmSourceStr = _webServer->arg("pwm_source_r_" + pwmIndexStr);

      String indexRealPwmStr = String(indexRealPwm);
      String pwmGpioNameStr = "pwm_gpio_r_" + indexRealPwmStr;
      String pwmSourceNameStr = "pwm_source_r_" + indexRealPwmStr;

      _espConfig->getDataStore()->addParameter(new DataParameter(pwmGpioNameStr.c_str(),pwmGpioStr.c_str()));
      _espConfig->getDataStore()->addParameter(new DataParameter(pwmSourceNameStr.c_str(),pwmSourceStr.c_str()));

      ++indexRealPwm;
    }
  }
  _spiffsManager->saveFile();
  sendResponseOk("PWM");
}

void WebConfig::handleAdcConfig()
{
  for (int cont = 0; cont < _webServer->args(); cont++)
  {
    Serial.println("Name: " + String(_webServer->argName(cont).c_str()));
    Serial.println("Value: " + String(_webServer->arg(cont).c_str()));
    _espConfig->getDataStore()->deleteParameter(_webServer->argName(cont).c_str());
    _espConfig->getDataStore()->addParameter(new DataParameter(_webServer->argName(cont).c_str(),_webServer->arg(cont).c_str()));
  }
  _spiffsManager->saveFile();
  sendResponseOk("ADC");
}

void WebConfig::handleManagementConfig()
{
  for (int cont = 0; cont < _webServer->args(); cont++)
  {
    Serial.println("Name: " + String(_webServer->argName(cont).c_str()));
    Serial.println("Value: " + String(_webServer->arg(cont).c_str()));
    _espConfig->getDataStore()->deleteParameter(_webServer->argName(cont).c_str());
    _espConfig->getDataStore()->addParameter(new DataParameter(_webServer->argName(cont).c_str(),_webServer->arg(cont).c_str()));
  }
  _spiffsManager->saveFile();
  sendResponseOk("Management");
}

void WebConfig::handleGpioConfig()
{
  _espConfig->getDataStore()->deleteParameterPrefix("gpio_mode_");
  _espConfig->getDataStore()->deleteParameterPrefix("gpio_comment_");
  for (int cont = 0; cont < _webServer->args(); cont++)
  {
    Serial.println("Name: " + String(_webServer->argName(cont).c_str()));
    Serial.println("Value: " + String(_webServer->arg(cont).c_str()));
    //_espConfig->getDataStore()->deleteParameter(_webServer->argName(cont).c_str());
    _espConfig->getDataStore()->addParameter(new DataParameter(_webServer->argName(cont).c_str(),_webServer->arg(cont).c_str()));
  }
  _spiffsManager->saveFile();
  sendResponseOk("GPIO");
}

void WebConfig::handleActionsConfig()
{
  int indexRealDigital = 0;
  int indexRealAdc = 0;
  _espConfig->getDataStore()->deleteParameterPrefix("action_");
  for (int cont = 0; cont < _webServer->args(); cont++)
  {
    String argNameStr = _webServer->argName(cont);

    if (argNameStr.indexOf("action_digital_gpio_origin_r_") >= 0)
    {
      String actionDigitalIndexStr = argNameStr.substring(29);
      String actionDigitalGpioOriginStr = _webServer->arg("action_digital_gpio_origin_r_" + actionDigitalIndexStr);
      String actionDigitalTriggerAnalisisTypeStr = _webServer->arg("action_digital_trigger_analisis_type_r_" + actionDigitalIndexStr);
      String actionDigitalActionStr = _webServer->arg("action_digital_action_r_" + actionDigitalIndexStr);
      String actionDigitalTimeBeforeActionReversalStr = _webServer->arg("action_digital_time_before_action_reversal_r_" + actionDigitalIndexStr);
      String actionDigitalGpioTargetStr = _webServer->arg("action_digital_gpio_target_r_" + actionDigitalIndexStr);
      String actionDigitalTelegramMessageStr = _webServer->arg("action_digital_telegram_message_r_" + actionDigitalIndexStr);
      String actionDigitalWaitingTimeRearmStr = _webServer->arg("action_digital_waiting_time_rearm_r_" + actionDigitalIndexStr);

      String indexRealDigitalStr = String(indexRealDigital);
      String actionDigitalGpioOriginNameStr = "action_digital_gpio_origin_r_" + indexRealDigitalStr;
      String actionDigitalTriggerAnalisisTypeNameStr = "action_digital_trigger_analisis_type_r_" + indexRealDigitalStr;
      String actionDigitalActionNameStr = "action_digital_action_r_" + indexRealDigitalStr;
      String actionDigitalTimeBeforeActionReversalNameStr = "action_digital_time_before_action_reversal_r_" + indexRealDigitalStr;
      String actionDigitalGpioTargetNameStr = "action_digital_gpio_target_r_" + indexRealDigitalStr;
      String actionDigitalTelegramMessageNameStr = "action_digital_telegram_message_r_" + indexRealDigitalStr;
      String actionDigitalWaitingTimeRearmNameStr = "action_digital_waiting_time_rearm_r_" + indexRealDigitalStr;
      

      _espConfig->getDataStore()->addParameter(new DataParameter(actionDigitalGpioOriginNameStr.c_str(),actionDigitalGpioOriginStr.c_str()));
      _espConfig->getDataStore()->addParameter(new DataParameter(actionDigitalTriggerAnalisisTypeNameStr.c_str(),actionDigitalTriggerAnalisisTypeStr.c_str()));
      _espConfig->getDataStore()->addParameter(new DataParameter(actionDigitalActionNameStr.c_str(),actionDigitalActionStr.c_str()));
      _espConfig->getDataStore()->addParameter(new DataParameter(actionDigitalTimeBeforeActionReversalNameStr.c_str(),actionDigitalTimeBeforeActionReversalStr.c_str()));
      _espConfig->getDataStore()->addParameter(new DataParameter(actionDigitalGpioTargetNameStr.c_str(),actionDigitalGpioTargetStr.c_str()));
      _espConfig->getDataStore()->addParameter(new DataParameter(actionDigitalTelegramMessageNameStr.c_str(),actionDigitalTelegramMessageStr.c_str()));
      _espConfig->getDataStore()->addParameter(new DataParameter(actionDigitalWaitingTimeRearmNameStr.c_str(),actionDigitalWaitingTimeRearmStr.c_str()));

      ++indexRealDigital;
    }
    if (argNameStr.indexOf("action_adc_gpio_origin_r_") >= 0)
    {
      String actionAdcIndexStr = argNameStr.substring(25);
      String actionAdcGpioOriginStr = _webServer->arg("action_adc_gpio_origin_r_" + actionAdcIndexStr);
      String actionAdcTriggerAnalisisTypeStr = _webServer->arg("action_adc_trigger_analisis_type_r_" + actionAdcIndexStr);
      String actionAdcTriggerValueStr = _webServer->arg("action_adc_trigger_value_r_" + actionAdcIndexStr);
      String actionAdcActionStr = _webServer->arg("action_adc_action_r_" + actionAdcIndexStr);
      String actionAdcGpioTargetStr = _webServer->arg("action_adc_gpio_target_r_" + actionAdcIndexStr);
      String actionAdcTelegramMessageStr = _webServer->arg("action_adc_telegram_message_r_" + actionAdcIndexStr);
      String actionAdcWaitingTimeRearmStr = _webServer->arg("action_adc_waiting_time_rearm_r_" + actionAdcIndexStr);
      String actionAdcTimeBeforeActionReversalStr = _webServer->arg("action_adc_time_before_action_reversal_r_" + actionAdcIndexStr);

      String indexRealAdcStr = String(indexRealAdc);
      String actionAdcGpioOriginNameStr = "action_adc_gpio_origin_r_" + indexRealAdcStr;
      String actionAdcTriggerAnalisisTypeNameStr = "action_adc_trigger_analisis_type_r_" + indexRealAdcStr;
      String actionAdcTriggerValueNameStr = "action_adc_trigger_value_r_" + indexRealAdcStr;
      String actionAdcActionNameStr = "action_adc_action_r_" + indexRealAdcStr;
      String actionAdcGpioTargetNameStr = "action_adc_gpio_target_r_" + indexRealAdcStr;
      String actionAdcTelegramMessageNameStr = "action_adc_telegram_message_r_" + indexRealAdcStr;
      String actionAdcWaitingTimeRearmNameStr = "action_adc_waiting_time_rearm_r_" + indexRealAdcStr;
      String actionAdcTimeBeforeActionReversalNameStr = "action_adc_time_before_action_reversal_r_" + indexRealAdcStr;

      _espConfig->getDataStore()->addParameter(new DataParameter(actionAdcGpioOriginNameStr.c_str(),actionAdcGpioOriginStr.c_str()));
      _espConfig->getDataStore()->addParameter(new DataParameter(actionAdcTriggerAnalisisTypeNameStr.c_str(),actionAdcTriggerAnalisisTypeStr.c_str()));
      _espConfig->getDataStore()->addParameter(new DataParameter(actionAdcTriggerValueNameStr.c_str(),actionAdcTriggerValueStr.c_str()));
      _espConfig->getDataStore()->addParameter(new DataParameter(actionAdcActionNameStr.c_str(),actionAdcActionStr.c_str()));
      _espConfig->getDataStore()->addParameter(new DataParameter(actionAdcGpioTargetNameStr.c_str(),actionAdcGpioTargetStr.c_str()));
      _espConfig->getDataStore()->addParameter(new DataParameter(actionAdcTelegramMessageNameStr.c_str(),actionAdcTelegramMessageStr.c_str()));
      _espConfig->getDataStore()->addParameter(new DataParameter(actionAdcWaitingTimeRearmNameStr.c_str(),actionAdcWaitingTimeRearmStr.c_str()));
      _espConfig->getDataStore()->addParameter(new DataParameter(actionAdcTimeBeforeActionReversalNameStr.c_str(),actionAdcTimeBeforeActionReversalStr.c_str()));

      ++indexRealAdc;
    }
  }
  _espConfig->getDataStore()->addParameter(new DataParameter("action_digital_total",String(indexRealDigital).c_str()));
  _espConfig->getDataStore()->addParameter(new DataParameter("action_adc_total",String(indexRealAdc).c_str()));
  _spiffsManager->saveFile();
  sendResponseOk("Actions");
}

void WebConfig::handleTelegramConfig()
{
  int indexRealTelegram = 0;
  _espConfig->getDataStore()->deleteParameterPrefix("telegram_");
  _espConfig->getDataStore()->addParameter(new DataParameter("telegram_enable",_webServer->arg("telegram_enable").c_str()));
  _espConfig->getDataStore()->addParameter(new DataParameter("telegram_token_id",_webServer->arg("telegram_token_id").c_str()));
  _espConfig->getDataStore()->addParameter(new DataParameter("telegram_client_id",_webServer->arg("telegram_client_id").c_str()));
  for (int cont = 0; cont < _webServer->args(); cont++)
  {
    String argNameStr = _webServer->argName(cont);
    if (argNameStr.indexOf("telegram_message_r_") >= 0)
    {
      String telegramMessageIndexStr = argNameStr.substring(19);
      String telegramMessageStr = _webServer->arg("telegram_message_r_" + telegramMessageIndexStr);
      String telegramGpioActionStr = _webServer->arg("telegram_gpio_action_r_" + telegramMessageIndexStr);
      String telegramGpioTargetStr = _webServer->arg("telegram_gpio_target_r_" + telegramMessageIndexStr);

      String indexRealTelegramStr = String(indexRealTelegram);
      String telegramMessageNameStr = "telegram_message_r_" + indexRealTelegramStr;
      String telegramGpioActionNameStr = "telegram_gpio_action_r_" + indexRealTelegramStr;
      String telegramGpioTargetNameStr = "telegram_gpio_target_r_" + indexRealTelegramStr;

      _espConfig->getDataStore()->addParameter(new DataParameter(telegramMessageNameStr.c_str(),telegramMessageStr.c_str()));
      _espConfig->getDataStore()->addParameter(new DataParameter(telegramGpioActionNameStr.c_str(),telegramGpioActionStr.c_str()));
      _espConfig->getDataStore()->addParameter(new DataParameter(telegramGpioTargetNameStr.c_str(),telegramGpioTargetStr.c_str()));

      ++indexRealTelegram;
    }
  }
  String telegramActionTotal = String(indexRealTelegram);
  _espConfig->getDataStore()->addParameter(new DataParameter("telegram_gpio_action_total",telegramActionTotal.c_str()));
  _spiffsManager->saveFile();
  sendResponseOk("Telegram");
}

void WebConfig::handleAlexaConfig()
{
  int indexRealAlexa = 0;
  _espConfig->getDataStore()->deleteParameterPrefix("alexa_");
  _espConfig->getDataStore()->addParameter(new DataParameter("alexa_enable",_webServer->arg("alexa_enable").c_str()));
  for (int cont = 0; cont < _webServer->args(); cont++)
  {
    String argNameStr = _webServer->argName(cont);
    if (argNameStr.indexOf("alexa_device_name_r_") >= 0)
    {
      String alexaMessageIndexStr = argNameStr.substring(20);
      String alexaMessageStr = _webServer->arg("alexa_device_name_r_" + alexaMessageIndexStr);
      String alexaGpioActionStr = _webServer->arg("alexa_support_dimmer_r_" + alexaMessageIndexStr);
      String alexaGpioTargetStr = _webServer->arg("alexa_gpio_target_r_" + alexaMessageIndexStr);

      String indexRealAlexaStr = String(indexRealAlexa);
      String alexaMessageNameStr = "alexa_device_name_r_" + indexRealAlexaStr;
      String alexaGpioActionNameStr = "alexa_support_dimmer_r_" + indexRealAlexaStr;
      String alexaGpioTargetNameStr = "alexa_gpio_target_r_" + indexRealAlexaStr;

      _espConfig->getDataStore()->addParameter(new DataParameter(alexaMessageNameStr.c_str(),alexaMessageStr.c_str()));
      _espConfig->getDataStore()->addParameter(new DataParameter(alexaGpioActionNameStr.c_str(),alexaGpioActionStr.c_str()));
      _espConfig->getDataStore()->addParameter(new DataParameter(alexaGpioTargetNameStr.c_str(),alexaGpioTargetStr.c_str()));

      ++indexRealAlexa;
    }
  }
  _spiffsManager->saveFile();
  sendResponseOk("Alexa");
}

void WebConfig::handleComponentsConfig()
{
  int indexRealDht = 0;
  int indexRealBmp180 = 0;
  int indexRealMcp3008 = 0;
  int indexRealSsd160 = 0;
  int indexRealSsd160Frame = 0;
  bool foundSsd160 = false;

  _espConfig->getDataStore()->deleteParameterPrefix("component_");

  for (int cont = 0; cont < _webServer->args(); cont++)
  {
    String argNameStr = _webServer->argName(cont);
    if (argNameStr.indexOf("component_dht_gpio_") >= 0)
    {
      String dhtMessageIndexStr = argNameStr.substring(19);
      String dhtMessageStr = _webServer->arg("component_dht_gpio_" + dhtMessageIndexStr);
      String dhtTypeStr = _webServer->arg("component_dht_type_" + dhtMessageIndexStr);

      String indexRealDhtStr = String(indexRealDht);
      String dhtMessageNameStr = "component_dht_gpio_" + indexRealDhtStr;
      String dhtTypeNameStr = "component_dht_type_" + indexRealDhtStr;

      _espConfig->getDataStore()->addParameter(new DataParameter(dhtMessageNameStr.c_str(),dhtMessageStr.c_str()));
      _espConfig->getDataStore()->addParameter(new DataParameter(dhtTypeNameStr.c_str(),dhtTypeStr.c_str()));
      ++indexRealDht;
    }
    else
    if (argNameStr.indexOf("component_bmp180_sda_") >= 0)
    {
      String bmp180MessageIndexStr = argNameStr.substring(21);
      String bmp180SdaMessageStr = _webServer->arg("component_bmp180_sda_" + bmp180MessageIndexStr);
      String bmp180SclMessageStr = _webServer->arg("component_bmp180_scl_" + bmp180MessageIndexStr);

      String indexRealBmp180Str = String(indexRealBmp180);
      String bmp180SdaMessageNameStr = "component_bmp180_sda_" + indexRealBmp180Str;
      String bmp180SclMessageNameStr = "component_bmp180_scl_" + indexRealBmp180Str;

      _espConfig->getDataStore()->addParameter(new DataParameter(bmp180SdaMessageNameStr.c_str(),bmp180SdaMessageStr.c_str()));
      _espConfig->getDataStore()->addParameter(new DataParameter(bmp180SclMessageNameStr.c_str(),bmp180SclMessageStr.c_str()));
      ++indexRealBmp180;
    }
    else
    if (argNameStr.indexOf("component_mcp3008_clock_") >= 0)
    {
      String mcp3008MessageIndexStr = argNameStr.substring(24);
      String mcp3008ClockMessageStr = _webServer->arg("component_mcp3008_clock_" + mcp3008MessageIndexStr);
      String mcp3008MisoMessageStr = _webServer->arg("component_mcp3008_miso_" + mcp3008MessageIndexStr);
      String mcp3008MosiMessageStr = _webServer->arg("component_mcp3008_mosi_" + mcp3008MessageIndexStr);
      String mcp3008CsMessageStr = _webServer->arg("component_mcp3008_cs_" + mcp3008MessageIndexStr);

      String indexRealMcp3008Str = String(indexRealMcp3008);
      String mcp3008ClockMessageNameStr = "component_mcp3008_clock_" + indexRealMcp3008Str;
      String mcp3008MisoMessageNameStr = "component_mcp3008_miso_" + indexRealMcp3008Str;
      String mcp3008MosiMessageNameStr = "component_mcp3008_mosi_" + indexRealMcp3008Str;
      String mcp3008CsMessageNameStr = "component_mcp3008_cs_" + indexRealMcp3008Str;

      _espConfig->getDataStore()->addParameter(new DataParameter(mcp3008ClockMessageNameStr.c_str(),mcp3008ClockMessageStr.c_str()));
      _espConfig->getDataStore()->addParameter(new DataParameter(mcp3008MisoMessageNameStr.c_str(),mcp3008MisoMessageStr.c_str()));
      _espConfig->getDataStore()->addParameter(new DataParameter(mcp3008MosiMessageNameStr.c_str(),mcp3008MosiMessageStr.c_str()));
      _espConfig->getDataStore()->addParameter(new DataParameter(mcp3008CsMessageNameStr.c_str(),mcp3008CsMessageStr.c_str()));

      ++indexRealMcp3008;
    }
    else
    if (argNameStr.indexOf("component_ssd160_sda_") >= 0)
    {
      foundSsd160 = true;
      String ssd160MessageIndexStr = argNameStr.substring(21);
      String ssd160SdaMessageStr = _webServer->arg("component_ssd160_sda_" + ssd160MessageIndexStr);
      String ssd160SclMessageStr = _webServer->arg("component_ssd160_scl_" + ssd160MessageIndexStr);

      String indexRealSsd160Str = String(indexRealSsd160);
      String ssd160SdaMessageNameStr = "component_ssd160_sda_" + indexRealSsd160Str;
      String ssd160SclMessageNameStr = "component_ssd160_scl_" + indexRealSsd160Str;

      _espConfig->getDataStore()->addParameter(new DataParameter(ssd160SdaMessageNameStr.c_str(),ssd160SdaMessageStr.c_str()));
      _espConfig->getDataStore()->addParameter(new DataParameter(ssd160SclMessageNameStr.c_str(),ssd160SclMessageStr.c_str()));
      ++indexRealSsd160;
    }
    else
    if ((argNameStr.indexOf("component_ssd160_frame_type_") >= 0) and (foundSsd160))
    {
      String ssd160FrameMessageIndexStr = argNameStr.substring(28);
      String ssd160FrameTypeMessageStr = _webServer->arg("component_ssd160_frame_type_" + ssd160FrameMessageIndexStr);
      String ssd160FrameDelayMessageStr = _webServer->arg("component_ssd160_frame_delay_" + ssd160FrameMessageIndexStr);

      String indexRealSsd160FrameStr = String(indexRealSsd160Frame);
      String ssd160FrameTypeMessageNameStr = "component_ssd160_frame_type_" + indexRealSsd160FrameStr;
      String ssd160FrameDelayMessageNameStr = "component_ssd160_frame_delay_" + indexRealSsd160FrameStr;

      _espConfig->getDataStore()->addParameter(new DataParameter(ssd160FrameTypeMessageNameStr.c_str(),ssd160FrameTypeMessageStr.c_str()));
      _espConfig->getDataStore()->addParameter(new DataParameter(ssd160FrameDelayMessageNameStr.c_str(),ssd160FrameDelayMessageStr.c_str()));
      ++indexRealSsd160Frame;
    }
  }
  _espConfig->getDataStore()->addParameter(new DataParameter("component_dht_total",String(indexRealDht)));
  _espConfig->getDataStore()->addParameter(new DataParameter("component_bmp180_total",String(indexRealBmp180)));
  _espConfig->getDataStore()->addParameter(new DataParameter("component_mcp3008_total",String(indexRealMcp3008)));
  _espConfig->getDataStore()->addParameter(new DataParameter("component_ssd160_total",String(indexRealSsd160)));
  _espConfig->getDataStore()->addParameter(new DataParameter("component_ssd160_frame_total",String(indexRealSsd160Frame)));
  _spiffsManager->saveFile();
  sendResponseOk("Components");
}

void WebConfig::handleWifiConfig()
{
  for (int cont = 0; cont < _webServer->args(); cont++)
  {
    Serial.println("Name: " + String(_webServer->argName(cont).c_str()));
    Serial.println("Value: " + String(_webServer->arg(cont).c_str()));
    _espConfig->getDataStore()->deleteParameter(_webServer->argName(cont).c_str());
    _espConfig->getDataStore()->addParameter(new DataParameter(_webServer->argName(cont).c_str(),_webServer->arg(cont).c_str()));
  }
  _spiffsManager->saveFile();
  sendResponseOk("WI-FI");
  scanWifi();
  WiFi.persistent(true);
  String wifiName = "";
  String wifiPassword = _espConfig->getDataStore()->getValue("access_point_password");

  if (strcmp(_espConfig->getDataStore()->getValue("access_point_manual"),"") != 0)
  {
    wifiName = _espConfig->getDataStore()->getValue("access_point_manual");
  }
  else
  if (strcmp(_espConfig->getDataStore()->getValue("access_point_select"),"") != 0)
  {
    wifiName = _espConfig->getDataStore()->getValue("access_point_select");
  }
  WiFi.begin(wifiName.c_str(),wifiPassword.c_str());
  WiFi.persistent(false);


}

void WebConfig::sendResponseOk(const char * fieldCChar)
{
  String responseText = FPSTR(RESPONSE_OK);
  responseText.replace("{::CONFIG_NAME::}",fieldCChar);
  _webServer->setContentLength(strlen_P(HEADER_EN_US)+responseText.length()+strlen_P(FOOTER_EN_US_P1)+
                               strlen_P(FOOTER_EN_US_P2));
  _webServer->send(200, FPSTR(HTTP_HEAD_CT_HTML),"");
  _webServer->sendContent_P(HEADER_EN_US);
  _webServer->sendContent(responseText);
  _webServer->sendContent_P(FOOTER_EN_US_P1);
  _webServer->sendContent_P(FOOTER_EN_US_P2);
}

void WebConfig::handleWifi()
{
  String accessPointManualStr = getStringFormatted(_espConfig->getDataStore()->getValue("access_point_manual"));
  String accessPointPassword = getStringFormatted(_espConfig->getDataStore()->getValue("access_point_password"));
  String ipAddressStr = getStringFormatted(_espConfig->getDataStore()->getValue("ip_address"));
  String ipMaskStr = getStringFormatted(_espConfig->getDataStore()->getValue("ip_mask"));
  String ipGatewayStr = getStringFormatted(_espConfig->getDataStore()->getValue("ip_gateway"));
  String ipDnsStr = getStringFormatted(_espConfig->getDataStore()->getValue("ip_dns"));

  int accessPointManualLength = accessPointManualStr.length();
  int accessPointPasswordLength = accessPointPassword.length();
  int ipAddressLength = ipAddressStr.length();
  int ipMaskLength = ipMaskStr.length();
  int ipGatewayLength = ipGatewayStr.length();
  int ipDnsLength = ipDnsStr.length();

  _webServer->setContentLength(strlen_P(HEADER_EN_US)+strlen_P(WIFI_EN_US_P1)+
                               _wifiListHtmlOptions.length()+strlen_P(WIFI_EN_US_P2)+accessPointManualLength+strlen_P(WIFI_EN_US_P3)+
                               accessPointPasswordLength+strlen_P(WIFI_EN_US_P4)+strlen_P(WIFI_EN_US_P5)+ipAddressLength+
                               strlen_P(WIFI_EN_US_P6)+ipMaskLength+strlen_P(WIFI_EN_US_P7)+ipGatewayLength+
                               strlen_P(WIFI_EN_US_P8)+ipDnsLength+strlen_P(WIFI_EN_US_P9)+strlen_P(FOOTER_EN_US_P1)+
                               strlen_P(FOOTER_EN_US_P2));
  //_webServer->setContentLength(CONTENT_LENGTH_UNKNOWN);
  _webServer->send(200, FPSTR(HTTP_HEAD_CT_HTML),"");
  _webServer->sendContent_P(HEADER_EN_US);
  _webServer->sendContent_P(WIFI_EN_US_P1);
  _webServer->sendContent(_wifiListHtmlOptions);
  _webServer->sendContent_P(WIFI_EN_US_P2);
  _webServer->sendContent(accessPointManualStr);
  _webServer->sendContent_P(WIFI_EN_US_P3);
  _webServer->sendContent(accessPointPassword);
  _webServer->sendContent_P(WIFI_EN_US_P4);
  _webServer->sendContent_P(WIFI_EN_US_P5);
  _webServer->sendContent(ipAddressStr);
  _webServer->sendContent_P(WIFI_EN_US_P6);
  _webServer->sendContent(ipMaskStr);
  _webServer->sendContent_P(WIFI_EN_US_P7);
  _webServer->sendContent(ipGatewayStr);
  _webServer->sendContent_P(WIFI_EN_US_P8);
  _webServer->sendContent(ipDnsStr);
  _webServer->sendContent_P(WIFI_EN_US_P9);

  _webServer->sendContent_P(FOOTER_EN_US_P1);
  _webServer->sendContent_P(FOOTER_EN_US_P2);
  // _webServer->client().stop();
}

void WebConfig::handleActions()
{
  int totalParameters = _espConfig->getDataStore()->getDataParametersCount();
  int actionDigitalLength = 0;
  int actionAdcLength = 0;
  int totaActionDigital = 0;
  int totaActionAdc = 0;

  for (int cont=0; cont<totalParameters; cont++)
  {
    if (_espConfig->getDataStore()->getParameterByPos(cont) != NULL)
    {
      String parameterLocal = _espConfig->getDataStore()->getParameterByPos(cont)->getField();
      if (parameterLocal.indexOf("action_digital_gpio_origin_r_") == 0)
      {
        ++totaActionDigital;
        String actionDigitalIndexStr = parameterLocal.substring(29);
        actionDigitalLength += getActionDigitalBody(actionDigitalIndexStr).length();
      }
      if (parameterLocal.indexOf("action_adc_gpio_origin_r_") == 0)
      {
        ++totaActionAdc;
        String actionAdcIndexStr = parameterLocal.substring(25);
        actionAdcLength += getActionAdcBody(actionAdcIndexStr).length();
      }
    }
  }
  String actionsJsP1 = FPSTR(ACTIONS_JS_EN_US_P1);
  actionsJsP1.replace("{::COUNTERDIGITAL::}",String(totaActionDigital));
  actionsJsP1.replace("{::COUNTERADC::}",String(totaActionAdc));
  _webServer->setContentLength(strlen_P(HEADER_EN_US)+strlen_P(ACTIONS_EN_US_P1)+actionDigitalLength+strlen_P(ACTIONS_EN_US_P2)+
                               strlen_P(ACTIONS_EN_US_P3)+actionAdcLength+strlen_P(ACTIONS_EN_US_P4)+
                               strlen_P(FOOTER_EN_US_P1)+actionsJsP1.length()+
                               strlen_P(ACTIONS_JS_EN_US_P2)+getGpioInputOptions(-1,"").length()+
                               getGpioOutputOptions(-1,"").length()+strlen_P(ACTIONS_JS_EN_US_P3)+
                               getGpioAdcOptions(-1,"").length()+strlen_P(ACTIONS_JS_EN_US_P4)+
                               getGpioOutputOptions(-1,"").length()+strlen_P(ACTIONS_JS_EN_US_P5)+
                               strlen_P(ACTIONS_JS_EN_US_P6)+strlen_P(FOOTER_EN_US_P2));
  _webServer->send(200, FPSTR(HTTP_HEAD_CT_HTML),"");
  _webServer->sendContent_P(HEADER_EN_US);
  _webServer->sendContent_P(ACTIONS_EN_US_P1);
  for (int cont=0; cont<totalParameters; cont++)
  {
    if (_espConfig->getDataStore()->getParameterByPos(cont) != NULL)
    {
      String parameterLocal = _espConfig->getDataStore()->getParameterByPos(cont)->getField();
      if (parameterLocal.indexOf("action_digital_gpio_origin_r_") == 0)
      {
        String actionDigitalIndexStr = parameterLocal.substring(29);
        _webServer->sendContent(getActionDigitalBody(actionDigitalIndexStr));
      }
    }
  }
  _webServer->sendContent_P(ACTIONS_EN_US_P2);
  _webServer->sendContent_P(ACTIONS_EN_US_P3);
  for (int cont=0; cont<totalParameters; cont++)
  {
    if (_espConfig->getDataStore()->getParameterByPos(cont) != NULL)
    {
      String parameterLocal = _espConfig->getDataStore()->getParameterByPos(cont)->getField();
      if (parameterLocal.indexOf("action_adc_gpio_origin_r_") == 0)
      {
        String actionAdcIndexStr = parameterLocal.substring(25);
        _webServer->sendContent(getActionAdcBody(actionAdcIndexStr));
      }
    }
  }
  _webServer->sendContent_P(ACTIONS_EN_US_P4);
  _webServer->sendContent_P(FOOTER_EN_US_P1);
  _webServer->sendContent(actionsJsP1);
  _webServer->sendContent_P(ACTIONS_JS_EN_US_P2);
  _webServer->sendContent(getGpioInputOptions(-1,""));
  _webServer->sendContent_P(ACTIONS_JS_EN_US_P3);
  _webServer->sendContent(getGpioOutputOptions(-1,""));
  _webServer->sendContent_P(ACTIONS_JS_EN_US_P4);
  _webServer->sendContent(getGpioAdcOptions(-1,""));
  _webServer->sendContent_P(ACTIONS_JS_EN_US_P5);
  _webServer->sendContent(getGpioOutputOptions(-1,""));
  _webServer->sendContent_P(ACTIONS_JS_EN_US_P6);
  _webServer->sendContent_P(FOOTER_EN_US_P2);
}

void WebConfig::handleStylesCss()
{
  int cssLenght = (strlen_P(STYLES_CSS_P1) + strlen_P(STYLES_CSS_P2) +
                   strlen_P(STYLES_CSS_P3) + strlen_P(STYLES_CSS_P4) +
                   strlen_P(STYLES_CSS_P5) + strlen_P(STYLES_CSS_P6) +
                   strlen_P(STYLES_CSS_P7) + strlen_P(STYLES_CSS_P8) +
                   strlen_P(STYLES_CSS_P9) + strlen_P(STYLES_CSS_P10) +
                   strlen_P(STYLES_CSS_P11) + strlen_P(STYLES_CSS_P12) +
                   strlen_P(STYLES_CSS_P13) + strlen_P(STYLES_CSS_P14) +
                   strlen_P(STYLES_CSS_P15) + strlen_P(STYLES_CSS_P16) +
                   strlen_P(STYLES_CSS_P17) + strlen_P(STYLES_CSS_P18) +
                   strlen_P(STYLES_CSS_P19) + strlen_P(STYLES_CSS_P20) +
                   strlen_P(STYLES_CSS_P21) + strlen_P(STYLES_CSS_P22) +
                   strlen_P(STYLES_CSS_P23) + strlen_P(STYLES_CSS_P24) +
                   strlen_P(STYLES_CSS_P25) + strlen_P(STYLES_CSS_P26));

  //_webServer->sendHeader(FPSTR(HTTP_HEAD_CL), String(cssLenght));
  _webServer->setContentLength(cssLenght);
  _webServer->send(200, FPSTR(HTTP_HEAD_CT_CSS),"");
  _webServer->sendContent_P(STYLES_CSS_P1);
  _webServer->sendContent_P(STYLES_CSS_P2);
  _webServer->sendContent_P(STYLES_CSS_P3);
  _webServer->sendContent_P(STYLES_CSS_P4);
  _webServer->sendContent_P(STYLES_CSS_P5);
  _webServer->sendContent_P(STYLES_CSS_P6);
  _webServer->sendContent_P(STYLES_CSS_P7);
  _webServer->sendContent_P(STYLES_CSS_P8);
  _webServer->sendContent_P(STYLES_CSS_P9);
  _webServer->sendContent_P(STYLES_CSS_P10);
  _webServer->sendContent_P(STYLES_CSS_P11);
  _webServer->sendContent_P(STYLES_CSS_P12);
  _webServer->sendContent_P(STYLES_CSS_P13);
  _webServer->sendContent_P(STYLES_CSS_P14);
  _webServer->sendContent_P(STYLES_CSS_P15);
  _webServer->sendContent_P(STYLES_CSS_P16);
  _webServer->sendContent_P(STYLES_CSS_P17);
  _webServer->sendContent_P(STYLES_CSS_P18);
  _webServer->sendContent_P(STYLES_CSS_P19);
  _webServer->sendContent_P(STYLES_CSS_P20);
  _webServer->sendContent_P(STYLES_CSS_P21);
  _webServer->sendContent_P(STYLES_CSS_P22);
  _webServer->sendContent_P(STYLES_CSS_P23);
  _webServer->sendContent_P(STYLES_CSS_P24);
  _webServer->sendContent_P(STYLES_CSS_P25);
  _webServer->sendContent_P(STYLES_CSS_P26);
}

void WebConfig::handleJQuery()
{
  int jsLenght = (strlen_P(JQUERY_3_3_1_P1) + strlen_P(JQUERY_3_3_1_P2) +
                  strlen_P(JQUERY_3_3_1_P3) + strlen_P(JQUERY_3_3_1_P4) +
                  strlen_P(JQUERY_3_3_1_P5) + strlen_P(JQUERY_3_3_1_P6) +
                  strlen_P(JQUERY_3_3_1_P7) + strlen_P(JQUERY_3_3_1_P8) +
                  strlen_P(JQUERY_3_3_1_P9) + strlen_P(JQUERY_3_3_1_P10) +
                  strlen_P(JQUERY_3_3_1_P11) + strlen_P(JQUERY_3_3_1_P12) +
                  strlen_P(JQUERY_3_3_1_P13) + strlen_P(JQUERY_3_3_1_P14) +
                  strlen_P(JQUERY_3_3_1_P15) + strlen_P(JQUERY_3_3_1_P16) +
                  strlen_P(JQUERY_3_3_1_P17) + strlen_P(JQUERY_3_3_1_P18) +
                  strlen_P(JQUERY_3_3_1_P19) + strlen_P(JQUERY_3_3_1_P20));
  _webServer->setContentLength(jsLenght);
  _webServer->send(200, FPSTR(HTTP_HEAD_CT_JS),"");
  _webServer->sendContent_P(JQUERY_3_3_1_P1);
  _webServer->sendContent_P(JQUERY_3_3_1_P2);
  _webServer->sendContent_P(JQUERY_3_3_1_P3);
  _webServer->sendContent_P(JQUERY_3_3_1_P4);
  _webServer->sendContent_P(JQUERY_3_3_1_P5);
  _webServer->sendContent_P(JQUERY_3_3_1_P6);
  _webServer->sendContent_P(JQUERY_3_3_1_P7);
  _webServer->sendContent_P(JQUERY_3_3_1_P8);
  _webServer->sendContent_P(JQUERY_3_3_1_P9);
  _webServer->sendContent_P(JQUERY_3_3_1_P10);
  _webServer->sendContent_P(JQUERY_3_3_1_P11);
  _webServer->sendContent_P(JQUERY_3_3_1_P12);
  _webServer->sendContent_P(JQUERY_3_3_1_P13);
  _webServer->sendContent_P(JQUERY_3_3_1_P14);
  _webServer->sendContent_P(JQUERY_3_3_1_P15);
  _webServer->sendContent_P(JQUERY_3_3_1_P16);
  _webServer->sendContent_P(JQUERY_3_3_1_P17);
  _webServer->sendContent_P(JQUERY_3_3_1_P18);
  _webServer->sendContent_P(JQUERY_3_3_1_P19);
  _webServer->sendContent_P(JQUERY_3_3_1_P20);
}

void WebConfig::handlePopper()
{
  int popperLenght = (strlen_P(POPPER_P1) + strlen_P(POPPER_P2) +
                      strlen_P(POPPER_P3) + strlen_P(POPPER_P4) +
                      strlen_P(POPPER_P5) + strlen_P(POPPER_P6));
  _webServer->setContentLength(popperLenght);
  _webServer->send(200, FPSTR(HTTP_HEAD_CT_JS),"");
  _webServer->sendContent_P(POPPER_P1);
  _webServer->sendContent_P(POPPER_P2);
  _webServer->sendContent_P(POPPER_P3);
  _webServer->sendContent_P(POPPER_P4);
  _webServer->sendContent_P(POPPER_P5);
  _webServer->sendContent_P(POPPER_P6);
}

void WebConfig::handleBootStrap()
{
  int bootstrapLenght = (strlen_P(BOOTSTRAP_P1) + strlen_P(BOOTSTRAP_P2) +
                         strlen_P(BOOTSTRAP_P3) + strlen_P(BOOTSTRAP_P4) +
                         strlen_P(BOOTSTRAP_P5) + strlen_P(BOOTSTRAP_P6) +
                         strlen_P(BOOTSTRAP_P7) + strlen_P(BOOTSTRAP_P8) +
                         strlen_P(BOOTSTRAP_P9) + strlen_P(BOOTSTRAP_P10) +
                         strlen_P(BOOTSTRAP_P11) + strlen_P(BOOTSTRAP_P12) +
                         strlen_P(BOOTSTRAP_P13) + strlen_P(BOOTSTRAP_P14) +
                         strlen_P(BOOTSTRAP_P15) + strlen_P(BOOTSTRAP_P16) +
                         strlen_P(BOOTSTRAP_P17));
  _webServer->setContentLength(bootstrapLenght);
  _webServer->send(200, FPSTR(HTTP_HEAD_CT_JS),"");
  _webServer->sendContent_P(BOOTSTRAP_P1);
  _webServer->sendContent_P(BOOTSTRAP_P2);
  _webServer->sendContent_P(BOOTSTRAP_P3);
  _webServer->sendContent_P(BOOTSTRAP_P4);
  _webServer->sendContent_P(BOOTSTRAP_P5);
  _webServer->sendContent_P(BOOTSTRAP_P6);
  _webServer->sendContent_P(BOOTSTRAP_P7);
  _webServer->sendContent_P(BOOTSTRAP_P8);
  _webServer->sendContent_P(BOOTSTRAP_P9);
  _webServer->sendContent_P(BOOTSTRAP_P10);
  _webServer->sendContent_P(BOOTSTRAP_P11);
  _webServer->sendContent_P(BOOTSTRAP_P12);
  _webServer->sendContent_P(BOOTSTRAP_P13);
  _webServer->sendContent_P(BOOTSTRAP_P14);
  _webServer->sendContent_P(BOOTSTRAP_P15);
  _webServer->sendContent_P(BOOTSTRAP_P16);
  _webServer->sendContent_P(BOOTSTRAP_P17);
}
