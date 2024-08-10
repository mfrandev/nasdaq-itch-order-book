#include <TradeNonCross.h>

#include <cstring>

#include <StockTradingAction.h>
#include <AddOrder.h>
#include <endian_utils.h>
#include <string_utils.h>

/**
 * Parse the Trade Non-Cross body from the binary message
 */
TradeNonCross* parseTradeNonCrossBody(const char* data) {
    static TradeNonCross tradeNonCross;
    std::size_t offset = 0;
    tradeNonCross.orderReferenceNumber = toHostEndianUpTo64(&data[offset], 8); // We know this is an 8 byte int
    offset += 8;
    std::memcpy(&(tradeNonCross.buySellIndicator), &data[offset], BUY_SELL_INDICATOR_SIZE);
    offset += 1;
    tradeNonCross.shares = toHostEndianUpTo64(&data[offset], 4); // We know this is a 4 byte int
    offset += 4;
    char stock[STOCK_SIZE];
    std::memcpy(&stock[0], &data[offset], STOCK_SIZE);
    tradeNonCross.stock = charStarToString(stock, STOCK_SIZE);
    stripWhitespaceFromCPPString(tradeNonCross.stock);
    offset += STOCK_SIZE;
    tradeNonCross.price = toHostEndianUpTo64(&data[offset], 4); // We know this is a 4 byte int
    offset += 4;
    tradeNonCross.matchNumber = toHostEndianUpTo64(&data[offset], 8); // We know this is an 8 byte int
    return &tradeNonCross;
}
