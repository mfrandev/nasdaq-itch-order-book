#ifndef TREXQUANTTAKEHOME_MESSAGES_CROSS_TRADE_H_
#define TREXQUANTTAKEHOME_MESSAGES_CROSS_TRADE_H_

#include <cstdint>
#include <string>
#include <memory>

// Struct for parsing the CrossTrade binary message
struct CrossTrade {
    uint64_t shares;
    std::string stock;
    uint32_t crossPrice;
    uint64_t matchNumber;
    char crossType;
};

std::shared_ptr<CrossTrade> parseCrossTradeBody(const char* data);

#endif // TREXQUANTTAKEHOME_MESSAGES_CROSS_TRADE_H_