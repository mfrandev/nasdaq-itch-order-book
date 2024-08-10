#include <SystemEvent.h>

#include <cstring>

/**
 * Parse the system event body
 */
SystemEvent* parseSystemEventBody(const char* data) {
    static SystemEvent systemEvent;
    std::size_t offset = 0;
    std::memcpy(&(systemEvent.eventCode), &data[offset], EVENT_CODE_SIZE);
    return &systemEvent;
}
