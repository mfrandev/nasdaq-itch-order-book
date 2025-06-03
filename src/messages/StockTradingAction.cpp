#include <StockTradingAction.h>

#include <cstring>

#include <string_utils.h>

/**
 * Parse the stock trading action body contents from the buffer
 */
StockTradingAction parseStockTradingActionBody(const char* data) {
    size_t offset = 0;

    std::string stock = std::string(&data[offset], STOCK_SIZE);
    stripWhitespaceFromCPPString(stock);
    offset += STOCK_SIZE;

    char tradingState = data[offset];
    offset += TRADING_STATE_SIZE;

    char reserved = data[offset];
    offset += RESERVED_SIZE;

    std::string reason = std::string(&data[offset], REASON_SIZE);
    stripWhitespaceFromCPPString(reason);

    return StockTradingAction(stock, tradingState, reserved, reason);
}