/**
 * @file DebugMessage.h
 * @author Fabricio Tamusiunas (fabricio@tamusiunas.com)
 * @brief Send messages to Serial or Syslog
 * @version 0.1
 * @date 2019-02-11
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#include "DebugMessage.h"

volatile int DebugMessage::_messageTotal = 0;
#ifdef DEBUG_MESSAGE_BUFFER
  volatile int DebugMessage::_debugMessageMax = DEBUG_MESSAGE_BUFFER;
#else
  volatile int DebugMessage::_debugMessageMax = 15;
#endif
volatile MessageStruct *DebugMessage::_firstMessage;
volatile MessageStruct *DebugMessage::_lastMessage;

DebugMessage::DebugMessage()
{
}

DebugMessage::DebugMessage(SyslogManager *syslogManager)
{
    _syslogManager = syslogManager;    
}

bool DebugMessage::handleSyslog()
{
  #ifdef SYSLOGDEBUG
  if (syslogManager != NULL)
  {
    
    if (_firstMessage != NULL)
    {
      while (_firstMessage != NULL)
      {
        this->syslogManager->sendMessage((char *)_firstMessage->message);

        volatile MessageStruct *old_message = _firstMessage;
        _firstMessage = _firstMessage->next;
        
        delete[] old_message->message;
        delete old_message;
        
      }
      _lastMessage = NULL;
      _messageTotal = 0;
    }

    return true;
  }
  #endif
  return false;
}

void DebugMessage::debug(String debugMessage)
{
    // to avoid trash
    String messageFormatted = "Log " + getPlatform() + " (" + String(WifiGetChipId()) 
      + ") - Uptime: " + String(millis()) +  " - " + formatString(debugMessage);

    #ifdef SERIALDEBUG
      Serial.println(messageFormatted);
    #endif
    #ifdef SYSLOGDEBUG
    if (_messageTotal < 15)
    {

      volatile MessageStruct *newMessage = new volatile MessageStruct;

      newMessage->message = new volatile char[messageFormatted.length() + 1];
      strcpy((char *)newMessage->message,messageFormatted.c_str());
      newMessage->next = NULL;

      if (_firstMessage == NULL)
      {
        _firstMessage = newMessage; // init the list
      }
      else
      {
        _lastMessage->next = newMessage; //_lastMessage-next receives new message
      }
      _lastMessage = newMessage; // _lastMessage is the new message
      ++_messageTotal;
    }
    #endif
}

String DebugMessage::getPlatform()
{
  #ifdef ESP8266
    return "ESP8266";
  #else
    return "ESP32";
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