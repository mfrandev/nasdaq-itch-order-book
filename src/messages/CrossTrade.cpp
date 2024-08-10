#include <CrossTrade.h>

#include <cstring>

#include <StockTradingAction.h>
#include <string_utils.h>
#include <endian_utils.h>

/**
 * Parse the cross trade body from the binary buffer
 */
CrossTrade* parseCrossTradeBody(const char* data) {
    static CrossTrade crossTrade;
    std::size_t offset = 0;
    crossTrade.shares = toHostEndianUpTo64(&data[offset], 8); // We know this is an 8 byte int
    offset += 8;
    char stock[STOCK_SIZE];
    std::memcpy(&stock[0], &data[offset], STOCK_SIZE);
    crossTrade.stock = charStarToString(stock, STOCK_SIZE);
    stripWhitespaceFromCPPString(crossTrade.stock);
    offset += STOCK_SIZE;
    crossTrade.crossPrice = toHostEndianUpTo64(&data[offset], 4); // We know this is a 4 byte int
    offset += 4;
    crossTrade.matchNumber = toHostEndianUpTo64(&data[offset], 8); // We know this is an 8 byte int
    offset += 8;
    std::memcpy(&(crossTrade.crossType), &data[offset], 1);
    return &crossTrade;
}
