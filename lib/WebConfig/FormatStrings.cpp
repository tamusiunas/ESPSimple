/**
 * @file FormatStrings.cpp
 * @author Fabricio Tamusiunas (fabricio@tamusiunas.com)
 * @brief Format web pages strings
 * @version 0.1
 * @date 2019-02-09
 * 
 * @copyright Copyright (c) 2019
 * 
 */

#include "WebConfig.h"

String WebConfig::getGpioInputOutputOptions(int ignoreGpioInt, String configParameterStr)
{
  String gpioInputOutputOptionsStr = "";
  for (int cont = 0; cont < _espConfig->getTotalGpio(); cont++)
  {
    if (ignoreGpioInt != cont)
    {
      if ((_espConfig->getPinGpioAvaliable()[cont] == 1) and (_espConfig->getPinGpioInOut()[cont] == 2))
      {
        String gpioSelected = "";
        if (configParameterStr == String(cont))
        {
          gpioSelected = "selected";
        }
        gpioInputOutputOptionsStr += "<option value=\"" + String(cont) +"\" " + gpioSelected + ">" + String(cont) + " (" +
                                      _espConfig->getPinGpioDesc()[cont] + ")</option>";
      }
    }
  }
 return (gpioInputOutputOptionsStr);
}

String WebConfig::getYesNoOptions(String configuredParameter)
{
  String selectedYes = "";
  if (strcmp(_espConfig->getDataStore()->getValue(configuredParameter.c_str()),"yes") == 0)
  {
    selectedYes = "selected";
  }
  return "<option value=\"no\">No</option> <option value=\"yes\" " + selectedYes + ">Yes</option>";
}

String WebConfig::getZeroCrossFrequencyOptions()
{
  String selected60Hz = "";
  String selectedAuto = "";

  if (strcmp(_espConfig->getDataStore()->getValue("pwm_zero_cross_frequency"),"60") == 0)
  {
    selected60Hz = "selected";
  }
  else
  if (strcmp(_espConfig->getDataStore()->getValue("pwm_zero_cross_frequency"),"auto") == 0)
  {
    selectedAuto = "selected";
  }
  return "<option value=\"50\">50 hz</option><option value=\"60\" " + selected60Hz +
         ">60 hz</option><option value=\"auto\" " + selectedAuto + ">Auto</option>";
}

String WebConfig::getGpioAdcOptions(int ignoreGpioInt, String configParameterStr)
{
  String gpioAdcOptionsStr = "";
  for (int cont = 0; cont < _espConfig->getTotalGpio(); cont++)
  {
    
    if (_espConfig->getPinAnalogOnly()[cont] == 1)
    {
      String adcSelected = "";
      String adcString = "a" + String(cont);
      if (configParameterStr == adcString)
        {
          adcSelected = "selected";
        }
        gpioAdcOptionsStr += "<option value=\"" + adcString +"\" " + adcSelected + "> Analog " + String(cont) + "</option>";
    }
    if (ignoreGpioInt != cont)
    {
      if (_espConfig->getPinGpioAdcNumberArray()[cont] >= 0)
      {
        String adcSelected = "";
        if (configParameterStr == String(cont))
        {
          adcSelected = "selected";
        }
        gpioAdcOptionsStr += "<option value=\"" + String(cont) +"\" " + adcSelected + ">" + String(cont) + " (" +
                                _espConfig->getPinGpioDesc()[cont] + ")</option>";
      }
    }
  }
 return (gpioAdcOptionsStr);
}

String WebConfig::getPwmZeroCrossOptions(String zeroCrossConfigured)
{
  String nonZeroCrossStr = "";
  if (zeroCrossConfigured.indexOf("nonzerocross") >= 0)
  {
    nonZeroCrossStr = "selected";
  }
  #ifdef ESP8266
  return "<option value=\"nonzerocross\" " + nonZeroCrossStr +
         ">Do not use Zero Cross</option>";
  #else
  return "<option value=\"zerocross\">Based on Zero Cross</option><option value=\"nonzerocross\" " + nonZeroCrossStr +
         ">Do not use Zero Cross</option>";
  #endif
}

String WebConfig::getOnOffOption(String digitalAnalisisTypeConfigured)
{
  String offStr = "";

  if (digitalAnalisisTypeConfigured.indexOf("off") >= 0)
  {
    offStr = "selected";
  }
  return "<option value=\"on\">On</option> <option value=\"off\" " + offStr + ">Off</option> ";
}

String WebConfig::getOnOffReversedOption(String digitalAnalisisTypeConfigured)
{
  String offStr = "";
  String reversedStr = "";

  if (digitalAnalisisTypeConfigured.indexOf("off") >= 0)
  {
    offStr = "selected";
  }
  else
  if (digitalAnalisisTypeConfigured.indexOf("reversed") >= 0)
  {
    reversedStr = "selected";
  }
  return "<option value=\"on\">On</option> <option value=\"off\" " + offStr + ">Off</option> "
  "<option value=\"reversed\" " + reversedStr + ">Reversed</option>";
}

String WebConfig::getOnOffReverseOption(String digitalAnalisisTypeConfigured)
{
  String offStr = "";
  String reverseStr = "";

  if (digitalAnalisisTypeConfigured.indexOf("off") >= 0)
  {
    offStr = "selected";
  }
  else
  if (digitalAnalisisTypeConfigured.indexOf("reverse") >= 0)
  {
    reverseStr = "selected";
  }
  return "<option value=\"on\">On</option> <option value=\"off\" " + offStr + ">Off</option> "
  "<option value=\"reverse\" " + reverseStr + ">Reverse</option>";
}

String WebConfig::getAdcAnalisisTypeOptions(String adcAnalisisTypeConfigured)
{
  String greaterStr = "";
  String lowerStr = "";
  if (adcAnalisisTypeConfigured.indexOf("greaterthan") >= 0)
  {
    greaterStr = "selected";
  }
  else
  if (adcAnalisisTypeConfigured.indexOf("lowerthan") >= 0)
  {
    lowerStr = "selected";
  }

  return "<option value=\"variation\">Variation</option> <option value=\"greaterthan\" " + greaterStr + ">Greater "
         "Than</option> <option value=\"lowerthan\" " + lowerStr + ">Lower Than</option>";
}

String WebConfig::getActionOptions(String actionOptionConfigured)
{
  String turnOnStr = "";
  String turnOffStr = "";
  String telegramStr = "";
  if (actionOptionConfigured.indexOf("on") >= 0)
  {
    turnOnStr = "selected";
  }
  else
  if (actionOptionConfigured.indexOf("off") >= 0)
  {
    turnOffStr = "selected";
  }
  else
  if (actionOptionConfigured.indexOf("sendmessagetelegram") >= 0)
  {
    telegramStr = "selected";
  }
  return "<option value=\"reverse\">Reverse Voltage</option> <option value=\"on\" " + turnOnStr + ">Turn on</option> "
  "<option value=\"off\" " + turnOffStr + ">Turn off</option> <option value=\"sendmessagetelegram\" " + telegramStr +
  ">Send Telegram Message</option>";
}

String WebConfig::getPwmBody(String indexPwmStr)
{
  String pwmGpioRStr = "pwm_gpio_r_" + indexPwmStr;
  String pwmSourceStr = "pwm_source_r_" + indexPwmStr;

  String pwmBody = "<tbody> <tr> <td class=\"align-middle\"> <label>GPIO</label> <select class=\"form-control h-25\" "
  "id=\"" + pwmGpioRStr + "\" name=\"" + pwmGpioRStr + "\">";
  pwmBody += getGpioOutputOptions(-1, _espConfig->getDataStore()->getValue(pwmGpioRStr.c_str()));
  pwmBody += "</select> </td> <td class=\"align-middle\"> <label>PWM Source</label> <select class=\"form-control "
  "h-25\" id=\"" + pwmSourceStr + "\" name=\"" + pwmSourceStr + "\">";
  pwmBody += getPwmZeroCrossOptions(_espConfig->getDataStore()->getValue(pwmSourceStr.c_str()));
  pwmBody += "</select> </td> <td class=\"align-middle\"><input type=\"button\" class=\"ibtnDel btn btn-primary my-2\""
  " value=\"Delete\"></td> </tr>";
  return pwmBody;
}

String WebConfig::getTelegramBody(String indexTelegram)
{
  String indexTelegramStr = String(indexTelegram);
  String telegramMessageStr = "telegram_message_r_" + indexTelegramStr;
  String telegramGpioActionStr = "telegram_gpio_action_r_" + indexTelegramStr;
  String telegramGpioTargetStr = "telegram_gpio_target_r_" + indexTelegramStr;

  String telegramBody = "<tr> <td class=\"align-middle\"><label>Message</label><input name=\"" + telegramMessageStr +
  "\" class=\"form-control h-25\" placeholder=\"\" maxlength=\"100\" id=\"" + telegramMessageStr + "\" value=\"" +
  getStringFormatted(_espConfig->getDataStore()->getValue(telegramMessageStr.c_str())) + "\"></td> "
  "<td class=\"align-middle\"> <label>Action</label> <select class=\"form-control h-25\" id=\"" + telegramGpioActionStr +
  "\" name=\"" + telegramGpioActionStr + "\">";
  telegramBody += getOnOffReversedOption(_espConfig->getDataStore()->getValue(telegramGpioActionStr.c_str()));
  telegramBody += "</select> <label><br>GPIO Target</label> <select class=\"form-control h-25\" id=\"" +
  telegramGpioTargetStr + "\" name=\"" + telegramGpioTargetStr + "\">";
  telegramBody += getGpioOutputOptions(-1,_espConfig->getDataStore()->getValue(telegramGpioTargetStr.c_str()));
  telegramBody += "</select> </td> <td class=\"align-middle\"><input type=\"button\" class=\"ibtnDel btn btn-primary "
  "my-2\" value=\"Delete\"></td> </tr>";
  return telegramBody;
}

String WebConfig::getAlexaBody(String indexAlexa)
{
  //Serial.println("getAlexaBody: " + indexAlexa);
  String indexAlexaStr = String(indexAlexa);
  String alexaMessageStr = "alexa_device_name_r_" + indexAlexaStr;
  String alexaGpioActionStr = "alexa_support_dimmer_r_" + indexAlexaStr;
  String alexaGpioTargetStr = "alexa_gpio_target_r_" + indexAlexaStr;

  String alexaBody = "<tr> <td class=\"align-middle\"><label>Device Name</label><input name=\"" + alexaMessageStr +
  "\" class=\"form-control h-25\" placeholder=\"\" maxlength=\"100\" id=\"" + alexaMessageStr + "\" value=\"" +
  getStringFormatted(_espConfig->getDataStore()->getValue(alexaMessageStr.c_str())) + "\"></td> "
  "<td class=\"align-middle\"> <label>Support Dimmer?</label> <select class=\"form-control h-25\" id=\"" + alexaGpioActionStr +
  "\" name=\"" + alexaGpioActionStr + "\">";
  alexaBody += getYesNoOptions(_espConfig->getDataStore()->getValue(alexaGpioActionStr.c_str()));
  alexaBody += "</select> <label><br>GPIO Target</label> <select class=\"form-control h-25\" id=\"" +
  alexaGpioTargetStr + "\" name=\"" + alexaGpioTargetStr + "\">";
  alexaBody += getGpioOutputOptions(-1,_espConfig->getDataStore()->getValue(alexaGpioTargetStr.c_str()));
  alexaBody += "</select> </td> <td class=\"align-middle\"><input type=\"button\" class=\"ibtnDel btn btn-primary "
  "my-2\" value=\"Delete\"></td> </tr>";
  return alexaBody;
}

String WebConfig::getComponentDht(String indexDht, String dhtTypeValueStr)
{
  String componentDhtGpioStr = "component_dht_gpio_" + indexDht; //input
  String componentDhtTypeStr = "component_dht_type_" + indexDht;
  String dhtStr = "<tr> <th scope=\"row\" class=\"align-middle\" style=\"\">DHT (11/22)</th> "
  "<td class=\"align-middle\">";
  dhtStr += "<label>DHT Type</label><select class=\"form-control h-25\" id=\"" +
  componentDhtTypeStr + "\" name=\"" + componentDhtTypeStr + "\">" + getComponentDhtType(dhtTypeValueStr) + "</select>";
  dhtStr += "<label><br />DHT GPIO</label> <select class=\"form-control h-25\" id=\"" +
  componentDhtGpioStr + "\" name=\"" + componentDhtGpioStr + "\">";
  dhtStr += getGpioInputOptions(-1,_espConfig->getDataStore()->getValue(componentDhtGpioStr.c_str()));
  dhtStr += "</select> </td> <td class=\"align-middle\"><input type=\"button\" class=\"ibtnDel-dht btn btn-primary "
  "my-2\" value=\"Delete\"></td> </tr>";
  return dhtStr;
}

String WebConfig::getComponentDhtType(String dhtTypeValueStr)
{
  String dht22Str = "";
  if (dhtTypeValueStr == "dht22")
  {
    dht22Str = "selected";
  }
  String dhtTypeStr = "<option value=\"dht11\">DHT11</option><option value=\"dht22\" " + dht22Str + ">DHT22</option>";
  return dhtTypeStr;
}

String WebConfig::getComponentBmp180(String indexBmp180)
{
  String indexBmp180Str = String(indexBmp180);
  String componentBmp180SdaStr = "component_bmp180_sda_" + indexBmp180Str; //input/output
  String componentBmp180SclStr = "component_bmp180_scl_" + indexBmp180Str; //output
  String bmp180Str = "<tr> <th scope=\"row\" class=\"align-middle\" style=\"\">BMP180</th> <td class=\"align-middle\">"
  " <label>BMP180 SDA</label> <select class=\"form-control h-25\" id=\"" + componentBmp180SdaStr + "\" "
  "name=\"" + componentBmp180SdaStr + "\">";
  bmp180Str += getGpioInputOutputOptions(-1,_espConfig->getDataStore()->getValue(componentBmp180SdaStr.c_str()));
  bmp180Str += "</select> <label><br>BMP180 SCL</label> <select class=\"form-control h-25\" "
  "id=\"" + componentBmp180SclStr + "\" name=\"" + componentBmp180SclStr + "\">";
  bmp180Str += getGpioOutputOptions(-1,_espConfig->getDataStore()->getValue(componentBmp180SclStr.c_str()));
  bmp180Str += "</select> </td> <td class=\"align-middle\"><input type=\"button\" class=\"ibtnDel-bmp180 btn "
  "btn-primary my-2\" value=\"Delete\"></td> </tr>";
  return bmp180Str;
}

String WebConfig::getComponentMcp3008(String indexMcp3008)
{
  String indexMcp3008Str = String(indexMcp3008);
  String componentMcp3008ClockStr = "component_mcp3008_clock_" + indexMcp3008Str; //output
  String componentMcp3008MisoStr = "component_mcp3008_miso_" + indexMcp3008Str; //input-output
  String componentMcp3008MosiStr = "component_mcp3008_mosi_" + indexMcp3008Str; //output
  String componentMcp3008CsStr = "component_mcp3008_cs_" + indexMcp3008Str; //output
  String mcp3008Str = "<tr> <th scope=\"row\" class=\"align-middle\" style=\"\">MCP3008</th> "
  "<td class=\"align-middle\"> <label>MCP3008 Clock GPIO</label> <select class=\"form-control h-25\" "
  "id=\"" + componentMcp3008ClockStr + "\" name=\"" + componentMcp3008ClockStr + "\">";
  mcp3008Str += getGpioOutputOptions(-1,_espConfig->getDataStore()->getValue(componentMcp3008ClockStr.c_str()));
  mcp3008Str += "</select> <label><br>MCP3008 MISO GPIO</label> <select class=\"form-control h-25\" "
  "id=\"" + componentMcp3008MisoStr + "\" name=\"" + componentMcp3008MisoStr + "\">";
  mcp3008Str += getGpioInputOutputOptions(-1,_espConfig->getDataStore()->getValue(componentMcp3008MisoStr.c_str()));
  mcp3008Str += "</select> <label><br>MCP3008 MOSI GPIO</label> <select class=\"form-control h-25\" "
  "id=\"" + componentMcp3008MosiStr + "\" name=\"" + componentMcp3008MosiStr + "\">";
  mcp3008Str += getGpioOutputOptions(-1,_espConfig->getDataStore()->getValue(componentMcp3008MosiStr.c_str()));
  mcp3008Str += "<label><br>MCP3008 CS GPIO</label> <select class=\"form-control h-25\" "
  "id=\"" + componentMcp3008CsStr + "\" name=\"" + componentMcp3008CsStr + "\">";
  mcp3008Str += "</select> </td> <td class=\"align-middle\"><input type=\"button\" class=\"ibtnDel-mcp3008 btn "
  "btn-primary my-2\" value=\"Delete\"></td> </tr>";
  return mcp3008Str;
}

String WebConfig::getComponentSsd160(String indexSsd160)
{
  String indexSsd160Str = String(indexSsd160);
  String componentSsd160SdaStr = "component_ssd160_sda_" + indexSsd160Str; //input-output
  String componentSsd160SclStr = "component_ssd160_scl_" + indexSsd160Str; // output
  String ssd160Str = "<tr> <th scope=\"row\" class=\"align-middle\" style=\"\">SSD160</th> <td class=\"align-middle\">"
  " <label>SDA</label> <select class=\"form-control h-25\" id=\"" + componentSsd160SdaStr + "\" name=\"" +
  componentSsd160SdaStr + "\">";
  ssd160Str += getGpioInputOutputOptions(-1,_espConfig->getDataStore()->getValue(componentSsd160SdaStr.c_str()));
  ssd160Str += "</select> <label><br>SCL</label> <select class=\"form-control h-25\" "
  "id=\"" + componentSsd160SclStr + "\" name=\"" + componentSsd160SclStr + "\">";
  ssd160Str += getGpioOutputOptions(-1,_espConfig->getDataStore()->getValue(componentSsd160SclStr.c_str()));
  ssd160Str += "</select> </td> <td class=\"align-middle\"><input type=\"button\" class=\"ibtnDel-ssd160 btn "
  "btn-primary my-2\" value=\"Delete\"></td> </tr>";
  return ssd160Str;
}

String WebConfig::getComponentSsd160Frame(String indexSsd160Frame)
{
  String indexSsd160FrameStr = String(indexSsd160Frame);
  String componentSsd160FrameTypeStr = "component_ssd160_frame_type_" + indexSsd160FrameStr;
  String componentSsd160FrameDelayStr = "component_ssd160_frame_delay_" + indexSsd160FrameStr;
  String frameTypeOtaSelected = "";
  if (String(_espConfig->getDataStore()->getValue(componentSsd160FrameTypeStr.c_str())) == "ota")
  {
    frameTypeOtaSelected = "selected";
  }
  String ssd160Frame = "<tr> <th scope=\"row\" class=\"align-middle\" style=\"\">Frame</th> "
  "<td class=\"align-middle\"> <label>Frame Type</label> <select class=\"form-control h-25\" "
  "id=\"" + componentSsd160FrameTypeStr + "\" name=\"" + componentSsd160FrameTypeStr + "\" value=\"" +
  getStringFormatted(_espConfig->getDataStore()->getValue(componentSsd160FrameTypeStr.c_str())) +
  "\"> <option value=\"mqtt\">MQTT</option>"
  " <option value=\"ota\" " + frameTypeOtaSelected + ">Over The Air (OTA) Update</option> </select> <label><br>Delay</label><input "
  "name=\"" + componentSsd160FrameDelayStr + "\" type=\"number\" class=\"form-control h-25\" placeholder=\"\" "
  "maxlength=\"10\" id=\"" + componentSsd160FrameDelayStr + "\" value=\"" +
  getStringFormatted(_espConfig->getDataStore()->getValue(componentSsd160FrameDelayStr.c_str())) + "\"> </td> "
  "<td class=\"align-middle\"><input type=\"button\" "
  "class=\"ibtnDel-ssd160-frames btn btn-primary my-2\" value=\"Delete\"></td> </tr>";
  return ssd160Frame;
}

String WebConfig::getActionDigitalBody(String indexDigital)
{
  String indexDigitalStr = String(indexDigital);
  String actionDigitalGpioOrigin = "action_digital_gpio_origin_r_" + indexDigitalStr;
  String actionDigitalTriggerAnalisisType = "action_digital_trigger_analisis_type_r_" + indexDigitalStr;
  String actionDigitalAction = "action_digital_action_r_" + indexDigitalStr;
  String actionDigitalTimeBeforeActionReversal = "action_digital_time_before_action_reversal_r_" + indexDigitalStr;
  String actionDigitalGpioTarget = "action_digital_gpio_target_r_" + indexDigitalStr;
  String actionDigitalTelegramMessage = "action_digital_telegram_message_r_" + indexDigitalStr;
  String actionDigitalWaitingTimeRearm = "action_digital_waiting_time_rearm_r_" + indexDigitalStr;

  String digitalBody = "<tr> <td scope=\"row\" class=\"align-middle\" style=\"\"> <label>GPIO</label> <select "
  "class=\"form-control h-25\" id=\"" + actionDigitalGpioOrigin + "\" name=\"" + actionDigitalGpioOrigin + "\">";
  digitalBody += getGpioInputOptions(-1,_espConfig->getDataStore()->getValue(actionDigitalGpioOrigin.c_str()));
  digitalBody += "</select> </td> <td class=\"align-middle\"> <label>Trigger Analisis Type</label> <select "
  "class=\"form-control h-25\" id=\"" + actionDigitalTriggerAnalisisType + "\" name=\"" +
  actionDigitalTriggerAnalisisType + "\">";
  digitalBody += getOnOffReversedOption(_espConfig->getDataStore()->getValue(actionDigitalTriggerAnalisisType.c_str()));
  digitalBody += "</select> <label><br>Action</label> <select class=\"form-control h-25\" id=\"" + actionDigitalAction +
  "\" name=\"" + actionDigitalAction + "\">";
  digitalBody += getActionOptions(_espConfig->getDataStore()->getValue(actionDigitalAction.c_str()));
  digitalBody += "</select>"; 
  digitalBody += "<label><br>Time Before Action Reversal</label><input name=\"" + actionDigitalTimeBeforeActionReversal +
  "\" class=\"form-control h-25\" placeholder=\"\" maxlength=\"10\" type=\"number\" id=\"" +  actionDigitalTimeBeforeActionReversal +
  "\" value=\"" + getStringFormatted(_espConfig->getDataStore()->getValue(actionDigitalTimeBeforeActionReversal.c_str())) + "\">"; 
  digitalBody += "<label><br>GPIO Target</label> <select class=\"form-control h-25\" id=\"" +
  actionDigitalGpioTarget + "\" name=\"" + actionDigitalGpioTarget + "\">";
  digitalBody += getGpioOutputOptions(-1,_espConfig->getDataStore()->getValue(actionDigitalGpioTarget.c_str()));
  digitalBody += "</select>";
  #ifdef ENABLE_TELEGRAM
  digitalBody += "<label><br>Telegram message (output)</label><input name=\"" + actionDigitalTelegramMessage +
  "\" class=\"form-control h-25\" placeholder=\"\" maxlength=\"100\" id=\"" + actionDigitalTelegramMessage + "\" "
  "value=\"" + getStringFormatted(_espConfig->getDataStore()->getValue(actionDigitalTelegramMessage.c_str())) + "\">";
  #endif

  digitalBody += "<label><br>Waiting Time Before "
  "Rearming Trigger (ms)</label><input name=\"" + actionDigitalWaitingTimeRearm + "\" type=\"number\" "
  "class=\"form-control h-25\" placeholder=\"\" maxlength=\"5\" id=\"" + actionDigitalWaitingTimeRearm +
  "\" value=\"" + getStringFormatted(_espConfig->getDataStore()->getValue(actionDigitalWaitingTimeRearm.c_str())) + "\">  </td> <td "
  "class=\"align-middle\"><input type=\"button\" class=\"ibtnDel-digital btn btn-primary my-2\" value=\"Delete\"></td>"
  " </tr>";
  return digitalBody;
}

String WebConfig::getActionAdcBody(String indexAdc)
{
  String indexAdcStr = String(indexAdc);
  String actionAdcGpioOrigin = "action_adc_gpio_origin_r_" + indexAdcStr;
  String actionAdcTriggerAnalisisType = "action_adc_trigger_analisis_type_r_" + indexAdcStr;
  String actionAdcTriggerValueAction = "action_adc_trigger_value_r_" + indexAdcStr;
  String actionAdcAction = "action_adc_action_r_" + indexAdcStr;
  String actionAdcGpioTarget = "action_adc_gpio_target_r_" + indexAdcStr;
  String actionAdcTelegramMessage = "action_adc_telegram_message_r_" + indexAdcStr;
  String actionAdcWaitingTimeRearm = "action_adc_waiting_time_rearm_r_" + indexAdcStr;
  String actionAdcTimeBeforeActionReversal = "action_adc_time_before_action_reversal_r_" + indexAdcStr;

  String adcBody = "<tr> <td scope=\"row\" class=\"align-middle\" style=\"\"> <label>GPIO</label> "
  "<select class=\"form-control h-25\" id=\"" + actionAdcGpioOrigin + "\" name=\"" + actionAdcGpioOrigin + "\">";
  adcBody += getGpioAdcOptions(-1,_espConfig->getDataStore()->getValue(actionAdcGpioOrigin.c_str()));
  adcBody += "</select> </td> <td class=\"align-middle\"> <label>Trigger Analisis Type</label> <select "
  "class=\"form-control h-25\" id=\"" + actionAdcTriggerAnalisisType + "\" name=\"" + actionAdcTriggerAnalisisType + "\">";
  adcBody += getAdcAnalisisTypeOptions(_espConfig->getDataStore()->getValue(actionAdcTriggerAnalisisType.c_str()));
  adcBody += "</select> <label><br>Value</label><input name=\"" + actionAdcTriggerValueAction + "\" type=\"number\" "
  "class=\"form-control h-25\" placeholder=\"\" maxlength=\"5\" id=\"" + actionAdcTriggerValueAction + "\" value=\"" +
  getStringFormatted(_espConfig->getDataStore()->getValue(actionAdcTriggerValueAction.c_str())) + "\">";
  adcBody += "<label><br>Action</label> <select class=\"form-control h-25\" id=\"" + actionAdcAction + "\" name=\"" + actionAdcAction + "\">";
  adcBody += getActionOptions(_espConfig->getDataStore()->getValue(actionAdcAction.c_str()));
  adcBody += "</select><label><br />Time Before Action Reversal</label><input name=\"" + actionAdcTimeBeforeActionReversal;
  adcBody += "\" class=\"form-control h-25\" placeholder=\"\" maxlength=\"10\" type=\"number\" id=\"" + actionAdcTimeBeforeActionReversal;
  adcBody += "\" value=\"" + getStringFormatted(_espConfig->getDataStore()->getValue(actionAdcTimeBeforeActionReversal.c_str())) + "\">";
  adcBody += "<label><br>GPIO Target</label> <select class=\"form-control h-25\" ";
  adcBody += "id=\"" + actionAdcGpioTarget + "\" name=\"" + actionAdcGpioTarget + "\">";
  adcBody += getGpioOutputOptions(-1,_espConfig->getDataStore()->getValue(actionAdcGpioTarget.c_str()));
  adcBody += "</select>";
  #ifdef ENABLE_TELEGRAM
  adcBody += "<label><br>Telegram message (output)</label><input name=\"" + actionAdcTelegramMessage + "\" "
  "class=\"form-control h-25\" placeholder=\"\" maxlength=\"100\" id=\"" + actionAdcTelegramMessage + "\" value=\"" +
  getStringFormatted(_espConfig->getDataStore()->getValue(actionAdcTelegramMessage.c_str())) + "\"> ";
  #endif
  adcBody += "<label><br>Waiting Time Before Rearming Trigger (ms)</label><input name=\"" + actionAdcWaitingTimeRearm + "\" "
  "type=\"number\" class=\"form-control h-25\" placeholder=\"\" maxlength=\"5\" id=\"" +
  actionAdcWaitingTimeRearm + "\" value=\"" +
  getStringFormatted(_espConfig->getDataStore()->getValue(actionAdcWaitingTimeRearm.c_str())) + "\">  </td> "
  "<td class=\"align-middle\"><input type=\"button\" class=\"ibtnDel-adc btn "
  "btn-primary my-2\" value=\"Delete\"></td> </tr>";
  return adcBody;
}

String WebConfig::getGpioInputOptions(int ignoreGpioInt, String configuredParameterStr)
{
  String gpioInputOptionsStr = "";
  for (int cont = 0; cont < _espConfig->getTotalGpio(); cont++)
  {
    String gpioSelected = "";
    if (configuredParameterStr == String(cont))
    {
      gpioSelected = "selected";
    }
    if (ignoreGpioInt != cont)
    {
      if ((_espConfig->getPinGpioAvaliable()[cont] == 1) and ((_espConfig->getPinGpioInOut()[cont] == 0) or
          (_espConfig->getPinGpioInOut()[cont] == 2)))
      {
        gpioInputOptionsStr += "<option value=\"" + String(cont) +"\" " + gpioSelected + ">" + String(cont) + " (" +
                                _espConfig->getPinGpioDesc()[cont] + ")</option>";
      }
    }
  }
 return (gpioInputOptionsStr);
}

String WebConfig::getNoneOption()
{
  String gpioInputOptionsStr = "<option value=\"none\">none</option>";
  return (gpioInputOptionsStr);
}


String WebConfig::getGpioOutputOptions(int ignoreGpioInt, String configuredParameterStr)
{
  String gpioOutputOptionsStr = "";
  for (int cont = 0; cont < _espConfig->getTotalGpio(); cont++)
  {
    String gpioSelected = "";
    if (configuredParameterStr == String(cont))
    {
      gpioSelected = "selected";
    }
    if (ignoreGpioInt != cont)
    {
      if ((_espConfig->getPinGpioAvaliable()[cont] == 1) and ((_espConfig->getPinGpioInOut()[cont] == 1) or
          (_espConfig->getPinGpioInOut()[cont] == 2)))
      {
        gpioOutputOptionsStr += "<option value=\"" + String(cont) +"\" " + gpioSelected + ">" + String(cont) + " (" +
                                 _espConfig->getPinGpioDesc()[cont] + ")</option>";
      }
    }
  }
 return (gpioOutputOptionsStr);
}

String WebConfig::getGpioInOutAdcRowSelect(int gpioNumber)
{
  String gpioSelectStr = "";
  // Serial.println("getPinAnalogOnly()[" + String(gpioNumber) + "]: " + String(_espConfig->getPinAnalogOnly()[gpioNumber]));
  if (_espConfig->getPinAnalogOnly()[gpioNumber] == 1)
  {
    String gpioAdcAttenStr = "gpio_adc_analog_only_" + String(gpioNumber);
    gpioSelectStr += "<tr> <th scope=\"row\" class=\"align-middle\" style=\"\">Analog " + String(gpioNumber) +
                    "</th> <td class=\"align-middle\"><label>Enable</label><select class=\"form-control h-25\" id=\"" +
                    gpioAdcAttenStr + "\" name=\"" + gpioAdcAttenStr + "\">";
    String adcOnlySelectedYes = "";
    if (strcmp(_espConfig->getDataStore()->getValue(gpioAdcAttenStr.c_str()),"yes") == 0)
    {
      adcOnlySelectedYes = "selected";
    }
    gpioSelectStr += "<option value=\"no\">No</option>";
    gpioSelectStr += "<option value=\"yes\" " + adcOnlySelectedYes + ">Yes</option>";
    gpioSelectStr += "</select></td> </tr>";
  }
  
  String gpioGpioStr = "gpio_mode_" + String(gpioNumber);
  String gpioCommentStr = "gpio_comment_" + String(gpioNumber);
  String gpioAdcAttenStr = "gpio_adc_atten_" + String(gpioNumber);
  String gpioDesc = _espConfig->getPinGpioDesc()[gpioNumber];
  int gpioInOutInt = _espConfig->getPinGpioInOut()[gpioNumber];
  if (_espConfig->getPinGpioAvaliable()[gpioNumber] == 1)
  {
    gpioSelectStr += "<tr> <th scope=\"row\" class=\"align-middle\" style=\"\">" + String(gpioNumber) + "(" + gpioDesc +
                    ")</th> <td class=\"align-middle\"><label>Mode</label><select class=\"form-control h-25\" id=\"" +
                    gpioGpioStr + "\" name=\"" + gpioGpioStr + "\"><option value=\"none\">None</option>";

    String inputSelectedStr = "";
    String inputPullUpSelectedStr = "";
    String outputSelectedStr = "";
    String adcSelectedStr = "";
    String adcAtten2_5Str = "";
    String adcAtten6Str = "";
    String adcAtten11Str = "";

    if (strcmp(_espConfig->getDataStore()->getValue(gpioAdcAttenStr.c_str()),"2.5") == 0)
    {
      adcAtten2_5Str = "selected";
    }
    else
    if (strcmp(_espConfig->getDataStore()->getValue(gpioAdcAttenStr.c_str()),"6") == 0)
    {
      adcAtten6Str = "selected";
    }
    else
    if (strcmp(_espConfig->getDataStore()->getValue(gpioAdcAttenStr.c_str()),"11") == 0)
    {
      adcAtten11Str = "selected";
    }

    if (strcmp(_espConfig->getDataStore()->getValue(gpioGpioStr.c_str()),"input_pullup") == 0)
    {
      inputPullUpSelectedStr = "selected";
    }
    else
    if (strcmp(_espConfig->getDataStore()->getValue(gpioGpioStr.c_str()),"input") == 0)
    {
      inputSelectedStr = "selected";
    }
    else
    if (strcmp(_espConfig->getDataStore()->getValue(gpioGpioStr.c_str()),"output") == 0)
    {
      outputSelectedStr = "selected";
    }
    else
    if (strcmp(_espConfig->getDataStore()->getValue(gpioGpioStr.c_str()),"adc") == 0)
    {
      adcSelectedStr = "selected";
    }

    if ((gpioInOutInt == 0) or (gpioInOutInt == 2))
    {
      gpioSelectStr += "<option value=\"input\" " + inputSelectedStr + ">INPUT</option>";
      gpioSelectStr += "<option value=\"input_pullup\" " + inputPullUpSelectedStr + ">INPUT_PULLUP</option>";
    }
    if ((gpioInOutInt == 1) or (gpioInOutInt == 2))
    {
      gpioSelectStr += "<option value=\"output\" " + outputSelectedStr + ">OUTPUT</option>";
    }
    #ifdef ESP32
    // if ESP32 then ADC will be used only with channel 1. Channel 2 needs Wi-Fi disabled :-(
    if ((_espConfig->getPinGpioAdcNumberArray()[gpioNumber] >= 0) and (_espConfig->getPinGpioAdcChannelArray()[gpioNumber] == 1))
    #else
    if (_espConfig->getPinGpioAdcNumberArray()[gpioNumber] >= 0)
    #endif
    {
      gpioSelectStr += "<option value=\"adc\" " + adcSelectedStr + ">ADC</option>";
    }
    gpioSelectStr += "</select><div class=\"form-group\">";
    #ifdef ESP32
    // ESP32 allows attenuation. That's great! 
    if ((_espConfig->getPinGpioAdcNumberArray()[gpioNumber] >= 0) and (_espConfig->getPinGpioAdcChannelArray()[gpioNumber] == 1))
    {
      gpioSelectStr += "<label><br />ADC Attenuation</label><select class=\"form-control h-25\" id=\"" + gpioAdcAttenStr + "\" name=\"" +
                       gpioAdcAttenStr + "\"><option value=\"0\">0 DB (1.1V)</option>"
                        "<option value=\"2.5\" " + adcAtten2_5Str + ">2.5 DB (1.5V)</option>"
                        "<option value=\"6\" " + adcAtten6Str + ">6 DB (2.2V)</option>"
                        "<option value=\"11\" " + adcAtten11Str + ">11 DB (3.9V)</option>"
                        "</select>";
    }
    #endif
    gpioSelectStr += " <div class=\"form-group\"><label><br>Comment</label>"
                     "<input name=\"" + gpioCommentStr + "\" class=\"form-control h-25\" placeholder=\"\" "
                     "maxlength=\"10\" id=\"" + gpioCommentStr + "\" value=\"" +
                     getStringFormatted(_espConfig->getDataStore()->getValue(gpioCommentStr.c_str())) + "\"></div> </div> </td> "
                     "</tr>";
  }
  return(gpioSelectStr);
}


String WebConfig::getStringFormatted(String stringReceived)
{
  String stringReceivedLocal = stringReceived;
  stringReceivedLocal.replace("\"","&quot;");
  return stringReceivedLocal;
}


String WebConfig::getGpioAdcRow(int gpioNumber)
{
  String gpioSelectAdcStr = "";
  String gpioGpioAdcEnableStr = "gpio_adc_enable_" + String(gpioNumber);
  String gpioGpioAdcEnableTriggerStr = "gpio_adc_enable_trigger_" + String(gpioNumber);
  String gpioGpioAdcThresholdStr = "gpio_adc_trigger_threshold_" + String(gpioNumber);
  String gpioDesc = _espConfig->getPinGpioDesc()[gpioNumber];
  if (_espConfig->getPinGpioAdcNumberArray()[gpioNumber] >= 0)
  {
    gpioSelectAdcStr += "<tr> <th scope=\"row\" class=\"align-middle\" style=\"\">" + String(gpioNumber) + " (" +
                        gpioDesc +")</th> <td class=\"align-middle\"><label>Enable ADC</label><select "
                        "class=\"form-control h-25\" id=\"" + gpioGpioAdcEnableStr + "\" name=\"" +
                        gpioGpioAdcEnableStr + "\"><option value=\"no\">No</option> <option value=\"yes\">Yes"
                        "</option> </select> <label><br>Enable Trigger</label><select class=\"form-control h-25\" "
                        "id=\"" + gpioGpioAdcEnableTriggerStr + "\" name=\"" + gpioGpioAdcEnableTriggerStr + "\">" +
                        "<option value=\"no\">No</option> <option value=\"yes\">Yes</option> </select> <label>"
                        "<br>Trigger Threshold</label><input type=\"number\" class=\"form-control\" name=\"" +
                        gpioGpioAdcThresholdStr +"\"> </td></tr>";
  }
  return gpioSelectAdcStr;
}

String WebConfig::getGpioActionRow(int gpioNumber)
{
  String gpioActionRowStr = "";
  String gpioGpioActionStr = "gpio_action_" + String(gpioNumber);
  String gpioGpioTargetStr = "gpio_target_" + String(gpioNumber);
  String gpioDesc = _espConfig->getPinGpioDesc()[gpioNumber];
  if (_espConfig->getPinGpioAvaliable()[gpioNumber] == 1)
  {
    gpioActionRowStr = "<tr><th scope=\"row\" class=\"align-middle\" style=\"\">" + String(gpioNumber) + " (" + gpioDesc +
                       ")</th><td class=\"align-middle\"><label>Action</label><select class=\"form-control h-25\""
                       "id=\"" + gpioGpioActionStr + "\" name=\"" + gpioGpioActionStr + "\">"
                       "<option value=\"none\">None</option>"
                       "<option value=\"reverse\">Reverse Voltage</option>"
                       "</select><label><br>GPIO Target</label>"
                       "<select class=\"form-control h-25\" id=\"" + gpioGpioTargetStr + "\" name=\"" +
                       gpioGpioTargetStr + "\">" + getGpioOutputOptions(gpioNumber,"") + "</select></td> </tr> ";
  }
  return(gpioActionRowStr);
}

String WebConfig::getGpioActionJS()
{
  String gpioActionJSStr = "";
  gpioActionJSStr += "cols += '<td scope=\"row\" class=\"align-middle\" style=\"\">";
  gpioActionJSStr += "<select class=\"form-control h-25\" id=\"gpio_origin_r_' + counter + '\" name=\"gpio_origin_r_' + counter + '\">";
  gpioActionJSStr += getGpioInputOptions(-1,"");
  gpioActionJSStr += "</select></td>';\n";
  gpioActionJSStr += "cols += '<td class=\"align-middle\"> <label>When GPIO Origin is</label> <select class=\"form-control h-25\""
  " id=\"gpio_status_r_' + counter + '\" name=\"gpio_status_r_' + counter + '\"> <option value=\"on\">On</option> "
  "<option value=\"off\">Off</option> <option value=\"reversed\">Reversed</option> </select> "
  "<label><br />Do the Action</label> <select class=\"form-control h-25\" id=\"gpio_action_r_' + counter + '\" "
  "name=\"gpio_action_r_' + counter + '\"> <option value=\"reverse\">Reverse Voltage</option> "
  "<option value=\"on\">Turn on</option> <option value=\"off\">Turn off</option> <option "
  "value=\"sendmessagetelegram\">Send Telegram Message</option></select> "
  "<label><br />For GPIO Target</label> <select class=\"form-control h-25\" id=\"gpio_target_r_' + counter + '\" "
  "name=\"gpio_target_r_' + counter + '\">";
  gpioActionJSStr += getGpioOutputOptions(-1,"");
  gpioActionJSStr += "</select>";
  #ifdef ENABLE_TELEGRAM
  gpioActionJSStr += "<label><br />Telegram message (output)</label><input name=\"telegram_message_r_' + counter + '\" class=\"form-control h-25\" placeholder=\"\" "
  "maxlength=\"100\" id=\"telegram_message_r_' + counter + '\">  </td>';\n";
  #endif
  gpioActionJSStr += "cols += '<td class=\"align-middle\"><input type=\"button\" class=\"ibtnDel btn btn-primary "
  "my-2\"  value=\"Delete\"></td>';\n";
  return gpioActionJSStr;
}
