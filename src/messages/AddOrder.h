#ifndef TREXQUANTTAKEHOME_MESSAGES_ADD_ORDER_H_
#define TREXQUANTTAKEHOME_MESSAGES_ADD_ORDER_H_

#include <string>
#include <cstdint>

const size_t BUY_SELL_INDICATOR_SIZE = 1;
// const size_t STOCK_SIZE = 8; Use the value from StockTradingAction.h

// Store the Add Order message body
struct AddOrder {
    uint64_t orderReferenceNumber;
    char buySellIndicator;
    uint32_t shares;
    std::string stock;
    uint32_t price;
};

AddOrder* parseAddOrderBody(const char* data);

#endif // TREXQUANTTAKEHOME_MESSAGES_ADD_ORDER_H_