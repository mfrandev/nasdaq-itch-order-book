#include <TradeNonCross.h>

#include <cstring>

#include <StockTradingAction.h>
#include <AddOrder.h>
#include <endian_utils.h>
#include <string_utils.h>

/**
 * Parse the Trade Non-Cross body from the binary message
 */
TradeNonCross parseTradeNonCrossBody(const char* data) {
    size_t offset = 0;
    uint64_t orderReferenceNumber = toHostEndianUpTo64(&data[offset], 8); // We know this is an 8 byte int
    offset += 8;
    char buySellIndicator = data[offset];
    offset += 1;
    uint32_t shares = toHostEndianUpTo64(&data[offset], 4); // We know this is a 4 byte int
    offset += 4;
    std::string stock = std::string(&data[offset], STOCK_SIZE);
    stripWhitespaceFromCPPString(stock);
    offset += STOCK_SIZE;
    uint32_t price = toHostEndianUpTo64(&data[offset], 4); // We know this is a 4 byte int
    offset += 4;
    uint64_t matchNumber = toHostEndianUpTo64(&data[offset], 8); // We know this is an 8 byte int
    return TradeNonCross(orderReferenceNumber, buySellIndicator, shares, stock, price, matchNumber);
}
