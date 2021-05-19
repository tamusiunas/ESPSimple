/**
 * @file DebugMessage.h
 * @author Fabricio Tamusiunas (fabricio@tamusiunas.com)
 * @brief Headers for DebugMessage
 * @version 0.1
 * @date 2019-02-11
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#ifndef Debug_h
#define Debug_h
#include "SyslogManager.h"
#include <list>
#include <iterator>
using namespace std;

struct MessageStruct {
    volatile char* message = NULL;
    volatile MessageStruct *next = NULL;
};
class DebugMessage
{
    public:

        DebugMessage();
        DebugMessage(SyslogManager *syslogManager);
        void debug(String debugMessage);
        bool handleSyslog();
        static SyslogManager *syslogManager;
        volatile static int _messageTotal;
        volatile static MessageStruct *_firstMessage;
        volatile static MessageStruct *_lastMessage;
        volatile static int _debugMessageMax;

    private:
        SyslogManager *_syslogManager = NULL;
        String formatString(String message);        
        String getPlatform();       
};

#endif