#ifndef NASDAQ_MESSAGES_CROSS_TRADE_H_
#define NASDAQ_MESSAGES_CROSS_TRADE_H_

#include <cstdint>
#include <string>

// Struct for parsing the CrossTrade binary message
struct CrossTrade {
    uint64_t shares;
    std::string stock;
    uint32_t crossPrice;
    uint64_t matchNumber;
    char crossType;
};

CrossTrade* parseCrossTradeBody(const char* data);

#endif // NASDAQ_MESSAGES_CROSS_TRADE_H_