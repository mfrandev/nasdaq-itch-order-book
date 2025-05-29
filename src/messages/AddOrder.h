#ifndef NASDAQ_MESSAGES_ADD_ORDER_H_
#define NASDAQ_MESSAGES_ADD_ORDER_H_

#include <string>
#include <cstdint>

constexpr size_t BUY_SELL_INDICATOR_SIZE = 1;
// constexpr size_t STOCK_SIZE = 8; Use the value from StockTradingAction.h

// Store the Add Order message body
struct AddOrder {
    uint64_t orderReferenceNumber;
    char buySellIndicator;
    uint32_t shares;
    std::string stock;
    uint32_t price;
};

AddOrder* parseAddOrderBody(const char* data);

#endif // NASDAQ_MESSAGES_ADD_ORDER_H_