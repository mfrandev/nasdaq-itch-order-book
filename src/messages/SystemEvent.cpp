#include <SystemEvent.h>

#include <time_utils.h>
#include <VWAPManager.h>

extern bool workFinished;

void SystemEvent::processMessage() const {
    switch(eventCode) {
        case EVENT_CODE_END_OF_MARKET_HOURS:
            closeMarket();
            break;
    }
}

/**
 * Parse the system event body
 */
SystemEvent* parseSystemEventBody(BinaryMessageHeader header, const char* data) {
    size_t offset = 0;
    char eventCode = data[offset];
    return new SystemEvent(std::move(header), eventCode);
}
