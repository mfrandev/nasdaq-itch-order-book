#include <time_utils.h>

#include <cmath>

/** 
 * Get the current period from a timestamp 
 */
uint8_t getCurrentPeriodFromTimestamp(uint64_t timestamp) {
    return floor((timestamp - startOfFirstPeriod) / NANOSECONDS_PER_HOUR);
}

bool isMarketClosed() {
    return currentPeriod >= NUMBER_OF_PERIODS_PER_DAY;
}
