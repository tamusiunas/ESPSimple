#include "DebugMessage.h"

DebugMessage::DebugMessage()
{
}

DebugMessage::DebugMessage(SyslogManager *syslogManager)
{
    _syslogManager = syslogManager;
}

void DebugMessage::debug(String debugMessage)
{
    String messageFormatted = formatString(debugMessage);
    #ifdef SERIALDEBUG
    #ifdef ESP8266
    Serial.println("Log ESP8266 (" + String(WifiGetChipId()) + ") - Uptime: " + String(millis()) +  " - " + messageFormatted);
    #endif
    #ifdef ESP32
    Serial.println("Log ESP32 (" + String(WifiGetChipId()) + ") - Uptime: " + String(millis()) +  " - " + messageFormatted);
    #endif
    #endif
    #ifdef SYSLOGDEBUG
    if (_syslogManager != NULL)
    {
        #ifdef ESP8266
        _syslogManager->sendMessage("Log ESP8266 (" + String(WifiGetChipId()) + ") - Uptime: " + String(millis()) +  " - " + messageFormatted);
        #endif
        #ifdef ESP32
        _syslogManager->sendMessage("Log ESP32 (" + String(WifiGetChipId()) + ") - Uptime: " + String(millis()) +  " - " + messageFormatted);
        #endif
    }
    #endif
}

String DebugMessage::formatString(String message)
{
  String newMessage = "";
  for (unsigned int cont = 0; cont < message.length() ; cont++)
  {
    if (((message.charAt(cont) >= 'a') and (message.charAt(cont) <= 'z')) or
        ((message.charAt(cont) >= 'A') and (message.charAt(cont) <= 'Z')) or
        ((message.charAt(cont) >= '0') and (message.charAt(cont) <= '9')) or
        (message.charAt(cont) == ' ') or (message.charAt(cont) == '-') or
        (message.charAt(cont) == ':') or (message.charAt(cont) == '_') or
        (message.charAt(cont) == '(') or (message.charAt(cont) == ')') or
        (message.charAt(cont) == '[') or (message.charAt(cont) == ']') or
        (message.charAt(cont) == '.') or (message.charAt(cont) == ';') or
        (message.charAt(cont) == '{') or (message.charAt(cont) == '}') or
        (message.charAt(cont) == '"') or (message.charAt(cont) == '\'') or
        (message.charAt(cont) == ','))
    {
      newMessage += message.charAt(cont);
    }
  }
  newMessage.replace("\n","");
  newMessage.replace("\r","");
  return newMessage;
}