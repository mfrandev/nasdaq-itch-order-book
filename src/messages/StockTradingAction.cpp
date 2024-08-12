#include <StockTradingAction.h>

#include <cstring>

#include <string_utils.h>

/**
 * Parse the stock trading action body contents from the buffer
 */
StockTradingAction* parseStockTradingActionBody(const char* data) {
    static StockTradingAction stockTradingAction;
    size_t offset = 0;

    // Store bytes for stock in a temp variable, copy over to a cpp string after
    char stock[STOCK_SIZE];
    std::memcpy(&stock[0], &data[offset], STOCK_SIZE);
    stockTradingAction.stock = charStarToString(stock, STOCK_SIZE);
    stripWhitespaceFromCPPString(stockTradingAction.stock);
    offset += STOCK_SIZE;

    std::memcpy(&(stockTradingAction.tradingState), &data[offset], TRADING_STATE_SIZE);
    offset += TRADING_STATE_SIZE;

    std::memcpy(&(stockTradingAction.reserved), &data[offset], RESERVED_SIZE);
    offset += RESERVED_SIZE;

    char reason[REASON_SIZE];
    std::memcpy(&reason[0], &data[offset], REASON_SIZE);
    stockTradingAction.reason = charStarToString(reason, REASON_SIZE);
    stripWhitespaceFromCPPString(stockTradingAction.reason);

    return &stockTradingAction;
}