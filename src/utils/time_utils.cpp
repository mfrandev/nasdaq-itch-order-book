#include <time_utils.h>

#include <cmath>

uint8_t currentPeriod = 0;
char isAfterhours = false;
bool workFinished = false;

/** 
 * Get the current period from a timestamp 
 */
uint8_t getCurrentPeriodFromTimestamp(uint64_t timestamp) {
    return floor((timestamp - startOfFirstPeriod) / NANOSECONDS_PER_HOUR);
}

bool isAfterHours() {
    return isAfterhours;
}

void closeMarket() {
    isAfterhours = true;
}

bool isWorkFinished() {
    return workFinished;
}
