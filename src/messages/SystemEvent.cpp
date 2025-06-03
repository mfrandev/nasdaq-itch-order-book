#include <SystemEvent.h>

#include <cstring>

/**
 * Parse the system event body
 */
SystemEvent parseSystemEventBody(const char* data) {
    size_t offset = 0;
    char eventCode = data[offset];
    return SystemEvent(eventCode);
}
