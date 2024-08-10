#include <SystemEvent.h>

#include <cstring>

/**
 * Parse the system event body
 */
std::shared_ptr<SystemEvent> parseSystemEventBody(const char* data) {
    static std::shared_ptr<SystemEvent> systemEvent = std::make_shared<SystemEvent>();
    std::size_t offset = 0;
    std::memcpy(&(systemEvent -> eventCode), &data[offset], EVENT_CODE_SIZE);
    return systemEvent;
}
