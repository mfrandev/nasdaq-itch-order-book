#ifndef TREXQUANTTAKEHOME_MESSAGES_ADD_ORDER_H_
#define TREXQUANTTAKEHOME_MESSAGES_ADD_ORDER_H_

#include <memory>
#include <string>
#include <cstdint>

const std::size_t BUY_SELL_INDICATOR_SIZE = 1;
// const std::size_t STOCK_SIZE = 8; Use the value from StockTradingAction.h

// Store the Add Order message body
struct AddOrder {
    uint64_t orderReferenceNumber;
    char buySellIndicator;
    uint32_t shares;
    std::string stock;
    uint32_t price;
};

std::shared_ptr<AddOrder> parseAddOrderBody(const char* data);

#endif // TREXQUANTTAKEHOME_MESSAGES_ADD_ORDER_H_