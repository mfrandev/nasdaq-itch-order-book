#ifndef NASDAQ_MESSAGES_ADD_ORDER_MPID_H_
#define NASDAQ_MESSAGES_ADD_ORDER_MPID_H_

#include <string>
#include <cstdint>

// constexpr size_t BUY_SELL_INDICATOR_SIZE = 1; Use the value from AddOrder.h
// constexpr size_t STOCK_SIZE = 8; Use the value from StockTradingAction.h
constexpr size_t ATTRIBUTION_SIZE = 4;

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

#endif // NASDAQ_MESSAGES_ADD_ORDER_MPID_H_