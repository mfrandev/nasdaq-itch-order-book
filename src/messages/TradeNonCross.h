#ifndef TREXQUANTTAKEHOME_MESSAGES_TRADE_NON_CROSS_H_
#define TREXQUANTTAKEHOME_MESSAGES_TRADE_NON_CROSS_H_

#include <cstdint>
#include <string>
#include <memory>

// const std::size_t BUY_SELL_INDICATOR_SIZE = 1; Initialized in AddOrder.h
// const std::size_t STOCK_SIZE           = 8; Initialized in StockTradingAction.h
// For parsing a Trade (non-cross) message
struct TradeNonCross {
    uint64_t orderReferenceNumber;
    char buySellIndicator;
    uint32_t shares;
    std::string stock;
    uint32_t price;
    uint64_t matchNumber;
};

std::shared_ptr<TradeNonCross> parseTradeNonCrossBody(const char* data);

#endif // TREXQUANTTAKEHOME_MESSAGES_TRADE_NON_CROSS_H_