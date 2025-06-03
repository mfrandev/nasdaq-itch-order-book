#include <ProcessMessage.h>

#include <cassert>
#include <fmt/format.h>

#include <VWAPManager.h>
#include <OrderBook.h>
#include <AddOrder.h>
#include <AddOrderMPID.h>
#include <BrokenTradeOrOrderExecution.h>
#include <CrossTrade.h>
#include <OrderCancel.h>
#include <OrderDelete.h>
#include <OrderExecuted.h>
#include <OrderExecutedWithPrice.h>
#include <OrderReplace.h>
#include <StockTradingAction.h>
#include <SystemEvent.h>
#include <TradeNonCross.h>

#include <time_utils.h>

extern uint8_t currentPeriod;

void ProcessMessage::parseAndProcessMessageBody(const char *data, size_t bytesToRead, const BinaryMessageHeader& header)
{

    // Check if the hour has changed and handle things appropriately
    uint8_t periodOfThisMessage = getCurrentPeriodFromTimestamp(header.getTimestamp());
    if (periodOfThisMessage != currentPeriod)
    {

        // No need to continue keeping track of reporting period if market hours are done
        if (periodOfThisMessage < NUMBER_OF_PERIODS_PER_DAY) {
            fmt::println("Timestamp: {} Moving from period {} to period {}", header.getTimestamp(), currentPeriod, periodOfThisMessage);
            currentPeriod = periodOfThisMessage;
        }
    }

    switch (header.getMessageType())
    {

    case MESSAGE_TYPE_ADD_ORDER_NO_MPID:
    {
        // if(!isAfterHours()) return;
        AddOrder *addOrder = parseAddOrderBody(data);
        OrderBook::getInstance().addToActiveOrders(addOrder -> orderReferenceNumber, header.getStockLocate(), addOrder -> shares, addOrder -> price);
        // fmt::println("1. Adding: {},{},{},{},{}", addOrder -> orderReferenceNumber, addOrder -> buySellIndicator, addOrder -> shares, addOrder -> stock, addOrder -> price);
    }
    break;
    case MESSAGE_TYPE_ADD_ORDER_WITH_MPID:
    {
        // if(!isAfterHours()) return;
        AddOrderMPID *addOrderMPID = parseAddOrderMPIDBody(data);
        OrderBook::getInstance().addToActiveOrders(addOrderMPID->orderReferenceNumber, header.getStockLocate(), addOrderMPID->shares, addOrderMPID->price);
        // fmt::println("2. Adding: {},{},{},{},{},{}", addOrderMPID -> orderReferenceNumber, addOrderMPID -> buySellIndicator, addOrderMPID -> shares, addOrderMPID -> stock, addOrderMPID -> price, addOrderMPID -> attribution);
    }
    break;
    case MESSAGE_TYPE_TRADE_BROKEN:
    {
        BrokenTradeOrOrderExecution *brokenTradeOrOrderExecution = parseBrokenTradeOrOrderExecutionBody(data);
        VWAPManager::getInstance().handleBrokenTrade(header.getStockLocate(), brokenTradeOrOrderExecution -> matchNumber);
        // fmt::println("broken trade!");
    }
    break;
    case MESSAGE_TYPE_TRADE_CROSS:
    {
        if(isAfterHours()) return;
        CrossTrade *crossTrade = parseCrossTradeBody(data);
        VWAPManager::getInstance().handleCrossTrade(header.getTimestamp(), header.getStockLocate(), crossTrade -> crossPrice, crossTrade -> shares, crossTrade -> matchNumber);
        // fmt::println("3. {},{},{},{},{}", crossTrade -> shares, crossTrade -> stock, crossTrade -> crossPrice, crossTrade -> matchNumber, crossTrade -> crossType);
    }
    break;
    case MESSAGE_TYPE_ORDER_CANCELLED:
    {
        // if(!isAfterHours()) return;
        OrderCancel *orderCancel = parseOrderCancelBody(data);
        OrderBook::getInstance().cancelActiveOrder(orderCancel -> orderReferenceNumber, orderCancel -> cancelledShares);
        // fmt::println("4. {},{}",orderCancel -> orderReferenceNumber, orderCancel -> cancelledShares);
    }
    break;
    case MESSAGE_TYPE_ORDER_DELETE:
    {
        // if(!isAfterHours()) return;
        OrderDelete *orderDelete = parseOrderDeleteBody(data);
        OrderBook::getInstance().deleteActiveOrder(orderDelete -> orderReferenceNumber);
        // fmt::println("5. Deleting: {}", orderDelete -> orderReferenceNumber);
    }
    break;
    case MESSAGE_TYPE_ORDER_EXECUTED:
    {
        if(isAfterHours()) return;
        OrderExecuted *orderExecuted = parseOrderExecutedBody(data);
        VWAPManager::getInstance().handleOrderExecuted(
            header.getTimestamp(), 
            header.getStockLocate(), 
            orderExecuted -> orderReferenceNumber, 
            orderExecuted -> executedShares, 
            orderExecuted -> matchNumber
        );
        // fmt::println("6. {},{},{}", orderExecuted -> orderReferenceNumber, orderExecuted -> executedShares, orderExecuted -> matchNumber);
    }
    break;
    case MESSAGE_TYPE_ORDER_EXECUTED_WITH_PRICE:
    {
        if(isAfterHours()) return;
        OrderExecutedWithPrice *orderExecutedWithPrice = parseOrderExecutedWithPriceBody(data);
        VWAPManager::getInstance().handleOrderExecutedWithPrice(
            header.getTimestamp(), 
            header.getStockLocate(), 
            orderExecutedWithPrice->orderReferenceNumber,
            orderExecutedWithPrice->executedShares,
            orderExecutedWithPrice->matchNumber,
            orderExecutedWithPrice->printable,
            orderExecutedWithPrice->executionPrice
        );
        // fmt::println("7. {},{},{},{},{}", orderExecutedWithPrice -> orderReferenceNumber, orderExecutedWithPrice -> executedShares, orderExecutedWithPrice -> matchNumber, orderExecutedWithPrice -> printable, orderExecutedWithPrice -> executionPrice);
    }
    break;
    case MESSAGE_TYPE_ORDER_REPLACE:
    {
        // if(!isAfterHours()) return;
        OrderReplace *orderReplace = parseOrderReplaceBody(data);
        OrderBook::getInstance().replaceActiveOrder(header.getStockLocate(), orderReplace->originalOrderReferenceNumber, orderReplace->newOrderReferenceNumber, orderReplace->shares, orderReplace->price);
        // fmt::println("8. {},{},{},{}", orderReplace -> originalOrderReferenceNumber, orderReplace -> newOrderReferenceNumber, orderReplace -> shares, orderReplace -> price);
    }
    break;
    // Messages we need to handle
    case MESSAGE_TYPE_STOCK_TRADING_ACTION:
    {
        // if(!isAfterHours()) return;
        StockTradingAction stockTradingAction = parseStockTradingActionBody(data);
        VWAPManager::getInstance().handleStockTradingAction(header.getStockLocate(), stockTradingAction.getStock(), stockTradingAction.getTradingState());
        // fmt::println("9. StockLocate {}: {},{},{},{}", header.getStockLocate(), stockTradingAction->stock, stockTradingAction->tradingState, stockTradingAction->reserved, stockTradingAction->reason);
    }
    break;
    case MESSAGE_TYPE_SYSTEM_EVENT:
    {
        SystemEvent systemEvent = parseSystemEventBody(data);
        // fmt::println("{}",systemEvent -> eventCode);
        // Prodce the output
        switch(systemEvent.getEventCode()) {
            case EVENT_CODE_END_OF_MARKET_HOURS:
                closeMarket();
                break;

            case EVENT_CODE_END_OF_SYSTEM_HOURS:
                VWAPManager::getInstance().outputBrokenTradeAdjustedVWAP();
                break;
        }
        
    }
    case MESSAGE_TYPE_TRADE_NON_CROSS:
    {
        if(isAfterHours()) return;
        TradeNonCross tradeNonCross = parseTradeNonCrossBody(data);
        VWAPManager::getInstance().handleTrade(header.getTimestamp(), header.getStockLocate(), tradeNonCross.getPrice(), tradeNonCross.getShares(), tradeNonCross.getMatchNumber());
        // fmt::println("10. {},{},{},{},{},{}", tradeNonCross.getOrderReferenceNumber(), tradeNonCross.getBuySellIndicator(), tradeNonCross.getShares(), tradeNonCross.geStock(), tradeNonCross.getPrice(), tradeNonCross.getMatchNumber());
    }
    break;

    default:
        break;
    }
}

/**
 * Given a message type, return how many bytes its body is
 */
size_t ProcessMessage::messageTypeToNumberOfBytes(char messageType)
{
    size_t messageSize;
    switch (messageType)
    {
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
        messageSize = MESSAGE_SIZE_OPERATIONAL_HALT;
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