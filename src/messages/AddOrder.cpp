#include <AddOrder.h>

#include <cstring>

#include <StockTradingAction.h>
#include <string_utils.h>
#include <endian_utils.h>

/** 
 * Parse the AddOrder message body
 */
AddOrder* parseAddOrderBody(const char* data) {
    static AddOrder addOrder;
    size_t offset = 0;
    addOrder.orderReferenceNumber = toHostEndianUpTo64(&data[offset], 8); // We know this is an 8 byte int
    offset += 8;
    std::memcpy(&(addOrder.buySellIndicator), &data[offset], BUY_SELL_INDICATOR_SIZE);
    offset += BUY_SELL_INDICATOR_SIZE;
    addOrder.shares = toHostEndianUpTo64(&data[offset], 4); // We know this is a 4 byte int
    offset += 4;
    char stock[STOCK_SIZE];
    std::memcpy(&stock[0], &data[offset], STOCK_SIZE);
    addOrder.stock = charStarToString(stock, STOCK_SIZE);
    stripWhitespaceFromCPPString(addOrder.stock);
    offset += STOCK_SIZE;
    addOrder.price = toHostEndianUpTo64(&data[offset], 4); // We know this is a 4 byte int
    return &addOrder;
}
