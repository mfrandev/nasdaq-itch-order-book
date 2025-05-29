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

// Struct for storing the System Event message body
struct SystemEvent {
    char eventCode;
};

SystemEvent* parseSystemEventBody(const char* data);

#endif // NASDAQ_MESSAGES_SYSTEM_EVENT_H_