#ifndef TREXQUANTTAKEHOME_MESSAGES_STOCK_TRADING_ACTION_H_
#define TREXQUANTTAKEHOME_MESSAGES_STOCK_TRADING_ACTION_H_

#include <string>
#include <cstdint>

// Trade state reason constants
const char TRADING_STATE_REASON_HALTED            = 'H';
const char TRADING_STATE_REASON_PAUSED            = 'P';
const char TRADING_STATE_REASON_QUOTATION         = 'Q';
const char TRADING_STATE_REASON_TRADING_ON_NASDAQ = 'T';

// Stock trading action message body
const std::size_t STOCK_SIZE         = 8;
const std::size_t TRADING_STATE_SIZE = 1;
const std::size_t RESERVED_SIZE      = 1;
const std::size_t REASON_SIZE        = 4;

struct StockTradingAction {
    std::string stock;
    char tradingState;
    char reserved;
    std::string reason;
};

StockTradingAction* parseStockTradingActionBody(const char* data);

#endif // TREXQUANTTAKEHOME_MESSAGES_STOCK_TRADING_ACTION_H_