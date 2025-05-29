#ifndef NASDAQ_MESSAGES_TRADE_NON_CROSS_H_
#define NASDAQ_MESSAGES_TRADE_NON_CROSS_H_

#include <cstdint>
#include <string>

// constexpr size_t BUY_SELL_INDICATOR_SIZE = 1; Initialized in AddOrder.h
// constexpr size_t STOCK_SIZE           = 8; Initialized in StockTradingAction.h
// For parsing a Trade (non-cross) message
struct TradeNonCross {
    uint64_t orderReferenceNumber;
    char buySellIndicator;
    uint32_t shares;
    std::string stock;
    uint32_t price;
    uint64_t matchNumber;
};

TradeNonCross* parseTradeNonCrossBody(const char* data);

#endif // NASDAQ_MESSAGES_TRADE_NON_CROSS_H_