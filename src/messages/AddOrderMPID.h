#ifndef TREXQUANTTAKEHOME_MESSAGES_ADD_ORDER_MPID_H_
#define TREXQUANTTAKEHOME_MESSAGES_ADD_ORDER_MPID_H_

#include <string>
#include <cstdint>

// const size_t BUY_SELL_INDICATOR_SIZE = 1; Use the value from AddOrder.h
// const size_t STOCK_SIZE = 8; Use the value from StockTradingAction.h
const size_t ATTRIBUTION_SIZE = 4;

// Store the Add Order MPID message body
struct AddOrderMPID {
    uint64_t orderReferenceNumber;
    char buySellIndicator;
    uint32_t shares;
    std::string stock;
    uint32_t price;
    std::string attribution;
};

AddOrderMPID* parseAddOrderMPIDBody(const char* data);

#endif // TREXQUANTTAKEHOME_MESSAGES_ADD_ORDER_MPID_H_