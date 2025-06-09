#include <time_utils.h>

#include <cmath>

uint8_t currentPeriod = 0;
bool workFinished = false;
uint64_t marketCloseTimestamp = UINT64_MAX;

/** 
 * Get the current period from a timestamp 
 */
uint8_t getCurrentPeriodFromTimestamp(uint64_t timestamp) {
    return floor((timestamp - startOfFirstPeriod) / NANOSECONDS_PER_HOUR);
}

bool isAfterHours(uint64_t timestamp) {
    return timestamp > marketCloseTimestamp;
}

void closeMarket(uint64_t timestamp) {
    marketCloseTimestamp = timestamp;
}

bool isWorkFinished() {
    return workFinished;
}
