/**
 * @file SyslogManager.h
 * @author Fabricio Tamusiunas (fabricio@tamusiunas.com)
 * @brief Syslog Manager
 * @version 0.1
 * @date 2019-02-11
 * 
 * @copyright Copyright (c) 2019
 * 
 */


#ifndef SyslogManager_h
#define SyslogManager_h

#include <Syslog.h>
#include <WiFiUdp.h>
#include "WiFiAliases.h"

class SyslogManager
{
  public:
    /**
     * @brief Construct a new Syslog Manager object
     * 
     */
    SyslogManager();

    /**
     * @brief Construct a new Syslog Manager object
     * 
     * @param syslogServer Syslog hostname ou IP address
     * @param syslogPort Syslog UDP port
     */
    SyslogManager(String syslogServer, uint16_t syslogPort);

    /**
     * @brief Destroy the Syslog Manager object
     * 
     */
    ~SyslogManager();

    /**
     * @brief Set the Syslog Server object
     * 
     * @param syslogServer Syslog hostname ou IP address
     */
    void setSyslogServer(String syslogServer);

    /**
     * @brief Set the Syslog Port object
     * 
     * @param syslogPort Syslog UDP port
     */
    void setSyslogPort(uint16_t syslogPort);

    /**
     * @brief Set the Syslog App Name object
     * 
     * @param appName Application name used to be identified when sending message to remote server
     */
    void setSyslogAppName(String appName);

    /**
     * @brief Set the Syslog Hostname object
     * 
     * @param hostName Hostname used to be identified when sending message to remote server
     */
    void setSyslogHostName(String hostName);

    /**
     * @brief Set the Syslog Facility object
     * 
     * @param syslogFacility Facility used to receive the message to remote server
     */
    void setSyslogFacility(uint16_t syslogFacility);

    /**
     * @brief Send message to remote server
     * 
     * @param syslogFacility Facility used to receive the message to remote server
     * @param appName Application name used to be identified when sending message to remote server
     * @param message Message
     */
    void sendMessage(uint16_t syslogFacility, String appName, String message);

    /**
     * @brief Send message to remote server
     * 
     * @param message Message
     */
    void sendMessage(String message);

    /**
     * @brief Send message to remote server
     * 
     * @param appName Application name used to be identified when sending message to remote server
     * @param message Message
     */
    void sendMessage(String appName, String message);

  private:
    String _syslogServer;
    String _appName;
    String _hostName;
    uint16_t _syslogPort;
    WiFiUDP _udpClient;
    uint16_t _syslogFacility;
};

#endif
