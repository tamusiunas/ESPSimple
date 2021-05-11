#ifndef Debug_h
#define Debug_h
#include "SyslogManager.h"

class DebugMessage
{
    public:
        DebugMessage();
        DebugMessage(SyslogManager *syslogManager);
        void debug(String debugMessage);
        static SyslogManager *syslogManager;

    private:
        SyslogManager *_syslogManager = NULL;
        String formatString(String message);
};

#endif