#ifndef TREXQUANTTAKEHOME_MESSAGES_SYSTEM_EVENT_H_
#define TREXQUANTTAKEHOME_MESSAGES_SYSTEM_EVENT_H_

#include <cstdint>

// System Event Codes
const char EVENT_CODE_START_OF_MESSAGES     = 'O';
const char EVENT_CODE_START_OF_SYSTEM_HOURS = 'S';
const char EVENT_CODE_START_OF_MARKET_HOURS = 'Q';
const char EVENT_CODE_END_OF_MARKET_HOURS   = 'M';
const char EVENT_CODE_END_OF_SYSTEM_HOURS   = 'E';
const char EVENT_CODE_END_OF_MESSAGES       = 'C';

const std::size_t EVENT_CODE_SIZE = 1;

// Struct for storing the System Event message body
struct SystemEvent {
    char eventCode;
};

SystemEvent* parseSystemEventBody(const char* data);

#endif // TREXQUANTTAKEHOME_MESSAGES_SYSTEM_EVENT_H_