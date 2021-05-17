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