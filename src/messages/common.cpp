#include <common.h>

#include <utils/endian_utils.h>

/**
 * Take a pre-loaded sequence of bytes and parse the ITCH header out of it
 * Use the same portion of memory to store the header, just because
 */
std::shared_ptr<BinaryMessageHeader> parseHeader(const char* data) {
    static std::shared_ptr<BinaryMessageHeader> header = std::make_shared<BinaryMessageHeader>();
    std::size_t offset = 0;
    header -> message_type = toHostEndianUpTo64(&data[offset], 1);
    offset+=1;
    header -> stock_locate = toHostEndianUpTo64(&data[offset], 2);
    offset+=2;
    header -> tracking_number = (&data[offset], 2);
    offset+=2;
    header -> timestamp = toHostEndianUpTo64(&data[offset], 6);
    return header;
}

size_t messageTypeTo(char messageType) {
    size_t messageSize;
    switch(messageType) {
        case MESSAGE_TYPE_SYSTEM_EVENT:
            messageSize = MESSAGE_SIZE_SYSTEM_EVENT;
            break;

        case MESSAGE_TYPE_STOCK_DIRECTORY:
            messageSize = MESSAGE_SIZE_STOCK_DIRECTORY;
            break;
        case MESSAGE_TYPE_STOCK_TRADING_ACTION:
            messageSize = MESSAGE_SIZE_STOCK_TRADING_ACTION;
            break;
        case MESSAGE_TYPE_REG_SHO:
            messageSize = MESSAGE_SIZE_REG_SHO;
            break;
        case MESSAGE_TYPE_MARKET_PARTICIPANT_POSITION:
            messageSize = MESSAGE_SIZE_MARKET_PARTICIPANT_POSITION;
            break;
        case MESSAGE_TYPE_MWCB_DECLINE_LEVEL:
            messageSize = MESSAGE_SIZE_MWCB_DECLINE_LEVEL;
            break;
        case MESSAGE_TYPE_MWCB_STATUS:
            messageSize = MESSAGE_SIZE_MWCB_STATUS;
            break;
        case MESSAGE_TYPE_QUOTING_PERIOD_UPDATE:
            messageSize = MESSAGE_SIZE_QUOTING_PERIOD_UPDATE;
            break;
        case MESSAGE_TYPE_LULD_AUCTION_COLLAR:
            messageSize = MESSAGE_SIZE_LULD_AUCTION_COLLAR;
            break;
        case MESSAGE_TYPE_OPERATIONAL_HALT:
            messageSize = MESSAGE_SIZE_OPERATIONAL_HALT ;
            break;

        case MESSAGE_TYPE_ADD_ORDER_NO_MPID:
            messageSize = MESSAGE_SIZE_ADD_ORDER_NO_MPID;
            break;
        case MESSAGE_TYPE_ADD_ORDER_WITH_MPID:
            messageSize = MESSAGE_SIZE_ADD_ORDER_WITH_MPID;
            break;

        case MESSAGE_TYPE_ORDER_EXECUTED:
            messageSize = MESSAGE_SIZE_ORDER_EXECUTED;
            break;
        case MESSAGE_TYPE_ORDER_EXECUTED_WITH_PRICE:
            messageSize = MESSAGE_SIZE_ORDER_EXECUTED_WITH_PRICE;
            break;
        case MESSAGE_TYPE_ORDER_CANCELLED:
            messageSize = MESSAGE_SIZE_ORDER_CANCELLED;
            break;
        case MESSAGE_TYPE_ORDER_DELETE:
            messageSize = MESSAGE_SIZE_ORDER_DELETE;
            break;
        case MESSAGE_TYPE_ORDER_REPLACE:
            messageSize = MESSAGE_SIZE_ORDER_REPLACE;
            break;

        case MESSAGE_TYPE_TRADE_NON_CROSS:
            messageSize = MESSAGE_SIZE_TRADE_NON_CROSS;
            break;
        case MESSAGE_TYPE_TRADE_CROSS:
            messageSize = MESSAGE_SIZE_TRADE_CROSS;
            break;
        case MESSAGE_TYPE_TRADE_BROKEN:
            messageSize = MESSAGE_SIZE_TRADE_BROKEN;
            break;

        case MESSAGE_TYPE_NOII:
            messageSize = MESSAGE_SIZE_NOII;
            break;

        case MESSAGE_TYPE_RPII:
            messageSize = MESSAGE_SIZE_RPII;
            break;

        case MESSAGE_TYPE_DLCR_PRICE_DISCOVERY:
            messageSize = MESSAGE_SIZE_DLCR_PRICE_DISCOVERY;
            break;

        default:
            messageSize = 0;
            break;
    }
    return messageSize;
}