#include <AddOrder.h>

#include <cstring>

#include <StockTradingAction.h>
#include <string_utils.h>

/** 
 * Parse the AddOrder message body
 */
std::shared_ptr<AddOrder> parseAddOrderBody(const char* data) {
    static std::shared_ptr<AddOrder> addOrder = std::make_shared<AddOrder>();
    std::size_t offset = 0;
    std::memcpy(&(addOrder -> orderReferenceNumber), &data[offset], 8); // We know this is an 8 byte int
    offset += 8;
    std::memcpy(&(addOrder -> buySellIndicator), &data[offset], BUY_SELL_INDICATOR_SIZE);
    offset += BUY_SELL_INDICATOR_SIZE;
    std::memcpy(&(addOrder -> shares), &data[offset], 4); // We know this is a 4 byte int
    offset += 4;
    char stock[STOCK_SIZE];
    std::memcpy(&stock[0], &data[offset], STOCK_SIZE);
    addOrder -> stock = charStarToString(stock, STOCK_SIZE);
    stripWhitespaceFromCPPString(addOrder -> stock);
    offset += STOCK_SIZE;
    std::memcpy(&(addOrder -> price), &data[offset], 4); // We know this is a 4 byte int
    return addOrder;
}
