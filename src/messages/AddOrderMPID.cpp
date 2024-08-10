#include <AddOrderMPID.h>

#include <cstring>

#include <StockTradingAction.h>
#include <AddOrder.h>
#include <string_utils.h>
#include <endian_utils.h>

/** 
 * Parse the AddOrderMPID message body
 */
std::shared_ptr<AddOrderMPID> parseAddOrderMPIDBody(const char* data) {
    static std::shared_ptr<AddOrderMPID> addOrderMPID = std::make_shared<AddOrderMPID>();
    std::size_t offset = 0;
    addOrderMPID -> orderReferenceNumber = toHostEndianUpTo64(&data[offset], 8); // We know this is an 8 byte int
    offset += 8;
    std::memcpy(&(addOrderMPID -> buySellIndicator), &data[offset], BUY_SELL_INDICATOR_SIZE);
    offset += BUY_SELL_INDICATOR_SIZE;
    addOrderMPID -> shares = toHostEndianUpTo64(&data[offset], 4); // We know this is a 4 byte int
    offset += 4;
    char stock[STOCK_SIZE];
    std::memcpy(&stock[0], &data[offset], STOCK_SIZE);
    addOrderMPID -> stock = charStarToString(stock, STOCK_SIZE);
    stripWhitespaceFromCPPString(addOrderMPID -> stock);
    offset += STOCK_SIZE;
    addOrderMPID -> price = toHostEndianUpTo64(&data[offset], 4); // We know this is a 4 byte int
    offset += 4;
    char attribution[ATTRIBUTION_SIZE];
    std::memcpy(&attribution[0], &data[offset], ATTRIBUTION_SIZE);
    addOrderMPID -> attribution = charStarToString(attribution, ATTRIBUTION_SIZE);
    stripWhitespaceFromCPPString(addOrderMPID -> attribution);
    return addOrderMPID;
}
