#ifndef NASDAQ_MESSAGES_SYSTEM_EVENT_H_
#define NASDAQ_MESSAGES_SYSTEM_EVENT_H_

#include <cstdint>
#include <stdlib.h>

// System Event Codes
constexpr char EVENT_CODE_START_OF_MESSAGES     = 'O';
constexpr char EVENT_CODE_START_OF_SYSTEM_HOURS = 'S';
constexpr char EVENT_CODE_START_OF_MARKET_HOURS = 'Q';
constexpr char EVENT_CODE_END_OF_MARKET_HOURS   = 'M';
constexpr char EVENT_CODE_END_OF_SYSTEM_HOURS   = 'E';
constexpr char EVENT_CODE_END_OF_MESSAGES       = 'C';

constexpr size_t EVENT_CODE_SIZE = 1;

// Class for storing the System Event message body
class SystemEvent {

    private:
        char eventCode;

    public:

        /**
         * Compiler will generate the default destuctor copy/move operator= and copy/move constructors
         */
        SystemEvent(char eventCode) : eventCode(eventCode) {}

        void setEventCode(char eventCode) { this -> eventCode = eventCode; }
        char getEventCode() const { return this -> eventCode; } 
};

SystemEvent parseSystemEventBody(const char* data);

#endif // NASDAQ_MESSAGES_SYSTEM_EVENT_H_