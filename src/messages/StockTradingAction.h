#ifndef NASDAQ_MESSAGES_STOCK_TRADING_ACTION_H_
#define NASDAQ_MESSAGES_STOCK_TRADING_ACTION_H_

#include <string>
#include <cstdint>

// Trade state reason constants
constexpr char TRADING_STATE_REASON_HALTED            = 'H';
constexpr char TRADING_STATE_REASON_PAUSED            = 'P';
constexpr char TRADING_STATE_REASON_QUOTATION         = 'Q';
constexpr char TRADING_STATE_REASON_TRADING_ON_NASDAQ = 'T';

// Stock trading action message body
constexpr size_t STOCK_SIZE         = 8;
constexpr size_t TRADING_STATE_SIZE = 1;
constexpr size_t RESERVED_SIZE      = 1;
constexpr size_t REASON_SIZE        = 4;

struct StockTradingAction {
    std::string stock;
    char tradingState;
    char reserved;
    std::string reason;
};

StockTradingAction* parseStockTradingActionBody(const char* data);

#endif // NASDAQ_MESSAGES_STOCK_TRADING_ACTION_H_