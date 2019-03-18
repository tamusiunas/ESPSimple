/**
 * @file SyslogManager.cpp
 * @author Fabricio Tamusiunas (fabricio@tamusiunas.com)
 * @brief Syslog Manager
 * @version 0.1
 * @date 2019-02-11
 * 
 * @copyright Copyright (c) 2019
 * 
 */

#include "SyslogManager.h"

SyslogManager::SyslogManager()
{
  _syslogPort = 554;
  _appName = "IOT-" + String(WifiGetChipId());
  _hostName = _appName;
  _syslogFacility = LOG_DEBUG;
}

SyslogManager::SyslogManager(String syslogServer, uint16_t syslogPort)
{
  _syslogServer = syslogServer;
  _syslogPort = syslogPort;
  _appName = "IOT-" + String(WifiGetChipId());
  _hostName = _appName;
  _syslogFacility = LOG_DEBUG;
}

SyslogManager::~SyslogManager()
{

}

void SyslogManager::setSyslogServer(String syslogServer)
{
  _syslogServer = syslogServer;
}

void SyslogManager::setSyslogPort(uint16_t syslogPort)
{
  _syslogPort = syslogPort;
}

void SyslogManager::setSyslogAppName(String appName)
{
  _appName = appName;
}

void SyslogManager::setSyslogHostName(String hostName)
{
  _hostName = hostName;
}

void SyslogManager::setSyslogFacility(uint16_t syslogFacility)
{
  _syslogFacility = syslogFacility;
}

void SyslogManager::sendMessage(uint16_t syslogFacility, String appName, String message)
{
  Syslog *syslog = new Syslog(_udpClient,_syslogServer.c_str(),_syslogPort,_hostName.c_str(),appName.c_str(),syslogFacility);
  syslog->log(syslogFacility,message);
  delete syslog;
}

void SyslogManager::sendMessage(String appName, String message)
{
  Syslog *syslog = new Syslog(_udpClient,_syslogServer.c_str(),_syslogPort,_hostName.c_str(),appName.c_str(),_syslogFacility);
  syslog->log(_syslogFacility,message);
  delete syslog;
}

void SyslogManager::sendMessage(String message)
{
  Syslog *syslog = new Syslog(_udpClient,_syslogServer.c_str(),_syslogPort,_hostName.c_str(),_appName.c_str(),_syslogFacility);
  syslog->log(_syslogFacility,message);
  delete syslog;
}
