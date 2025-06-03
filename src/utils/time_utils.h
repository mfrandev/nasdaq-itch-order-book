#ifndef NASDAQ_UTILS_TIME_UTILS_H_
#define NASDAQ_UTILS_TIME_UTILS_H_

#include <cstdint>

constexpr static uint64_t NANOSECONDS_PER_HOUR      = 3600000000000;

/** 
 * 1. 9:30 - 10:00
 * 2. 10:00 - 11:00
 * 3. 11:00 - 12:00
 * 4. 12:00 - 13:00
 * 5. 13:00 - 14:00
 * 6. 14:00 - 15:00
 * 7. 15:00 - 16:00
 */
constexpr static uint64_t NUMBER_OF_PERIODS_PER_DAY = 7;

// 9:00
constexpr uint64_t startOfFirstPeriod = 32400000000000;
extern uint8_t currentPeriod;

/** 
 * Take a timestamp and get the period which it falls in 
 */
uint8_t getCurrentPeriodFromTimestamp(uint64_t timestamp);


bool isAfterHours();
void closeMarket();

#endif // NASDAQ_UTILS_TIME_UTILS_H_